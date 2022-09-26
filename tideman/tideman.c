#include "cs50.h"
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
    // TODO
    for (int k = 0; k < candidate_count; k++)
    {
        if (strcmp(name, candidates[k]) == 0)
        {
            bool write_in = true;
            for (int l = 0; l < rank; l++)
            {
                if (ranks[l] == k)
                {
                    write_in = false;
                    break;
                }
            }
            if (write_in)
            {
                ranks[rank] = k;
                return true;
            }
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int j = 0; j < candidate_count - 1; j++)
    {
        for (int k = j+1; k < candidate_count; k++)
        {
            preferences[ranks[j]][ranks[k]] += 1;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count += 1;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count += 1;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    //sort pairs
    for (int i = 0; i < pair_count - 1; i++)
    {
        int index = i;
        int most_winner = pairs[i].winner;
        int most_loser = pairs[i].loser;
        for (int j = i + 1; j < pair_count; j++)
        {
            if (preferences[most_winner][most_loser] < preferences[pairs[j].winner][pairs[j].loser])
            {
                index = j;
                most_winner = pairs[j].winner;
                most_loser = pairs[j].loser;
            }
        }
        // exchange the array when the refernce is not the most victory
        if (index != i)
        {
            pairs[index].winner = pairs[i].winner;
            pairs[index].loser = pairs[i].loser;
            pairs[i].winner = most_winner;
            pairs[i].loser = most_loser;

        }
    }
    for (int i = 0; i < pair_count; i++)
    {
        printf("pairs %i, %i is %i\n", pairs[i].winner, pairs[i].loser, preferences[pairs[i].winner][pairs[i].loser]);
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    //lock the strongest pairs
    bool keep_on = true;
    int order = 0;
    int sorted_list[candidate_count];
    int sorted_count = 0;
    int reference_winner = pairs[0].winner;
    int reference_loser = pairs[0].loser;

    //add first list
    locked[reference_winner][reference_loser] = 1;
    sorted_list[sorted_count] = reference_winner;
    sorted_count += 1;
    sorted_list[sorted_count] = reference_loser;
    sorted_count += 1;
    order += 1;

    //lock series
    if (sorted_count < candidate_count)
    {
        keep_on = true;
        while (keep_on)
        {
            for (int i = order; i < pair_count; i++)
            {
                if (pairs[i].winner == reference_loser)
                {
                    bool down_fit = true;
                    for (int j = 0; j < sorted_count; j++)
                    {
                        if (pairs[i].loser == sorted_list[j])
                        {
                            down_fit = false;
                            break;
                        }
                    }
                    if (down_fit)
                    {
                        locked[reference_loser][pairs[i].loser] = true;
                        reference_loser = pairs[i].loser;
                        sorted_list[sorted_count] = reference_loser;
                        sorted_count += 1;
                        break;
                    }
                }
                if (pairs[i].loser == reference_winner)
                {
                    bool up_fit = true;
                    for (int j = 0; j < sorted_count; j++)
                    {
                        if (pairs[i].winner == sorted_list[j])
                        {
                            up_fit = false;
                            break;
                        }
                    }
                    if (up_fit)
                    {
                        locked[pairs[i].winner][reference_winner] = true;
                        reference_winner = pairs[i].winner;
                        sorted_list[sorted_count] = reference_winner;
                        sorted_count += 1;
                        break;
                    }
                }
            }
            order += 1;
            if (sorted_count == candidate_count || order == pair_count)
            {
                keep_on = false;
            }
        }
    }
    string winner = candidates[reference_winner];
    printf("The winner is %s\n", winner);

    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    return;
}