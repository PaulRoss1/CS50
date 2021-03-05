
SELECT movies.title
FROM movies
WHERE movies.id IN (
    SELECT stars.movie_id
    FROM stars
    JOIN people
    ON stars.person_id = people.id
    WHERE people.name LIKE '%Helena Bonham Carter%'
) AND movies.id IN (
    SELECT stars.movie_id
    FROM stars
    JOIN people
    ON stars.person_id = people.id
    WHERE people.name LIKE '%Johnny Depp%'
);










