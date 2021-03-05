#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{

    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (!strcmp(name, candidates[i]))
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
// the struc pref groups together rank_index and voters_rank
// where rank_index runs from 1 upwards and voters_rank = ranks of each voter
    typedef struct
    {
        int rank_index;
        int voters_rank;
    }
    pref;

    pref prefs[candidate_count];

    for (int i = 0; i < candidate_count; i++)
    {
        prefs[i].rank_index = i + 1;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        prefs[i].voters_rank = ranks[i];
    }

    for (int x = 0; x < candidate_count; x++)
    {
        for (int y = 0; y < candidate_count; y++)
        {
// for preferences[i][j] if i = j than preferences[i][j] will be set to 0 so that it won't be added to pairs array in next step
            if (prefs[x].rank_index == prefs[y].rank_index)
            {
                preferences[prefs[x].voters_rank][prefs[y].voters_rank] = 0;
            }
// if one rank_index is lower than the oter rank index - the preferences array will be updated by +1
            else if (prefs[x].rank_index < prefs[y].rank_index)
            {
                preferences[prefs[x].voters_rank][prefs[y].voters_rank] ++;
            }
        }
    }

    return;
}



// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
// if there isn't a tie - pair_count is updated by +1
            if ((preferences[i][j] - preferences[j][i]) > 0)
            {
                pair_count ++;
            }
        }
    }

    int p = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
// updating the pairs array with the winner being i when preferences[i][j] is higher than preferences[j][i] and the loser being j
            if ((preferences[i][j] - preferences[j][i]) != 0 && preferences[i][j] > preferences[j][i])
            {
                pairs[p].winner = i;
                pairs[p].loser = j;
                p++;
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
// x is set to 0, iterating through the preferences if one is higher than x, x will become that preference - find the strongest preference
    int x = 0;
    int p = 0;
// once the strongerst preference is found it is saved as pairs[p].winner and pairs[p].looser
// that preference is than cleared so we can than find the second strongest preference
// also the counter p is updated by +1 so the nexter pairs[p].winner will be increased by one

    for (int k = 0; k < pair_count; k ++)
    {
        x = 0;

        for (int i = 0; i < candidate_count; i++)
        {
            for (int j = 0; j < candidate_count; j++)
            {
                if (preferences[i][j] > x)
                {
                    x = preferences[i][j];
                }
            }
        }

        for (int i = 0; i < candidate_count; i++)
        {
            for (int j = 0; j < candidate_count; j++)
            {
                if (preferences[i][j] == x)
                {
                    pairs[p].winner = i;
                    pairs[p].loser = j;
                    preferences[i][j] = 0;
                    p++;
                }
            }
        }
    }
    return;
}


// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
// by now I know that the winner is the person with the strongest preference - if anyone beats him - it will create a cycle
// keep updating the locked array unless someone beats the person with the strongest preference (pairs[0].winner)

    int a = pairs[0].winner;

    for (int i = 0; i < pair_count; i++)
    {
        if (pairs[i].loser != a)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}


// Print the winner of the election
void print_winner(void)
{
// sort preferences from strongest to weakest
    int a = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > a)
            {
                a = preferences[i][j];
            }
        }
    }
// print out the candidate with the strongest preference
// in case of tie, also print out the candidate with a preference strenght of one less
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if ((preferences[i][j] = a) || (preferences[i][j] = a - 1))
            {
                printf("%s\n", candidates[i]);
            }
        }
    }
}

