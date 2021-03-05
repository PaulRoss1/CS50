import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
import datetime
from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    try:
        db.execute("CREATE TABLE purchases(user_id int, symbol varchar(255), name varchar(255), shares int, price float, total float, time varchar(255))")
    except RuntimeError:
        pass
    purchase = db.execute("SELECT * FROM purchases WHERE user_id = :user_id", user_id=session["user_id"])
    cash = db.execute("SELECT SUM(total) FROM purchases")[0]["SUM(total)"]
    size = len(purchase)
    try:
        db.execute("CREATE TABLE history(user_id int, symbol varchar(255), shares int, price float, time varchar(255))")
        return render_template("index.html", purchase=purchase, cash=cash, size=size)
    except RuntimeError:
        return render_template("index.html", purchase=purchase, cash=cash, size=size)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        symbol = request.form.get("symbol")
        if lookup(symbol) == None or not symbol:
            return apology("symbol missing or invalid", 400)
        try:
            shares = int(request.form.get("shares"))
            if not shares:
                return apology("missing shares", 400)
            if shares < 1:
                return apology("shares must be a positive number", 400)
        except ValueError:
            return apology("shares must be a positive number", 400)

        user = db.execute("SELECT * FROM users WHERE id = :user_id", user_id=session["user_id"])
        price = float(lookup(symbol)["price"])
        share = int(request.form.get("shares"))
        cash = user[0]["cash"]
        name = lookup(symbol)["name"]
        if price*share > cash:
            return apology("can't afford", 400)
        try:
            db.execute("CREATE TABLE purchases(user_id int, symbol varchar(255), name varchar(255), shares int, price float, total float, time varchar(255))")
        except RuntimeError:
            purchase = db.execute("SELECT * FROM purchases WHERE user_id = :user_id", user_id=session["user_id"])
            for purchase in purchase:
                if symbol == purchase["symbol"]:
                    db.execute("UPDATE purchases SET shares = shares + :share, total = total + :total WHERE symbol=:symbol",
                    share=share, symbol=symbol, total=price*share)
                    db.execute("INSERT INTO history (user_id, symbol, shares, price, time) VALUES (:user_id, :symbol, :shares, :price, :time)",
                        user_id=session["user_id"], symbol=symbol, shares=share, price=price, time=str(datetime.datetime.now())[:19])
                    return redirect("/")
            else:
                db.execute("INSERT INTO purchases (user_id, symbol, name, shares, price, total, time) VALUES (:id, :symbol, :name, :shares, :price, :total, :time)",
                    id=user[0]["id"], symbol=symbol, name=name, shares=share, price=price, total=price*share, time=str(datetime.datetime.now())[:19])
                db.execute("INSERT INTO history (user_id, symbol, shares, price, time) VALUES (:user_id, :symbol, :shares, :price, :time)",
                        user_id=session["user_id"], symbol=symbol, shares=share, price=price, time=str(datetime.datetime.now())[:19])
        return redirect("/")

        db.execute("CREATE TABLE history(user_id int, symbol varchar(255), shares int, price float, time varchar(255))")




@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute("SELECT * FROM history WHERE user_id = :user_id", user_id=session["user_id"])
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    else:
        symbol = request.form.get("symbol")
        answer = lookup(symbol)
        if answer == None:
            return apology("invalid symbol", 400)
        return render_template("quoted.html", answer=answer)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    else:
        username = request.form.get("username")
        if not username:
            return apology("must provide username", 403)
        password = request.form.get("password")
        if not password:
            return apology("must provide a password", 403)
        confirmation = request.form.get("confirmation")
        if not confirmation:
            return apology("must confirm password", 403)
        if password != confirmation:
            return apology("passwords do not match", 403)
        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))
        if len(rows) == 1:
            return apology("username already exists", 403)
        db.execute("INSERT INTO users (username, hash, cash) VALUES (:username, :password, 10000)", username=username, password=generate_password_hash(password))
        return redirect("/")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    db.execute("DELETE FROM purchases WHERE shares=0")
    symbols = db.execute("SELECT DISTINCT symbol FROM purchases WHERE user_id = :user_id", user_id=session["user_id"])
    if request.method == "GET":
        return render_template("sell.html", symbols=symbols)
    else:
        shares = request.form.get("shares")
        symbol = request.form.get("symbol")
        price = float(lookup(symbol)["price"])
        purchase = db.execute("SELECT * FROM purchases WHERE user_id = :user_id and symbol = :symbol" , user_id=session["user_id"], symbol=symbol)
        if int(shares) > int(purchase[0]["shares"]):
            return apology("too many shares", 400)
        db.execute("UPDATE purchases SET shares = shares - :share, total = total - :total WHERE symbol=:symbol",
            share=shares, symbol=symbol, total = int(shares)*price)
        db.execute("INSERT INTO history (user_id, symbol, shares, price, time) VALUES (:user_id, :symbol, :shares, :price, :time)",
                        user_id=session["user_id"], symbol=symbol, shares=-int(shares), price=price, time=str(datetime.datetime.now())[:19])
    return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
