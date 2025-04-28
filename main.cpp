#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
using namespace std;

// Constants for color codes and formatting
const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";

class VotingSystem
{
private:
    struct Candidate
    {
        string name;
        string party;
        int votes = 0;
    };

    vector<Candidate> candidates;
    vector<string> votedVoters;
    const string adminPassword = "admin123";

    // File paths
    const string CANDIDATES_FILE = "candidates.txt";
    const string VOTERS_FILE = "voters.txt";

    void drawTableHeader(const string &title)
    {
        cout << "\n+" << string(58, '-') << "+\n";
        cout << "| " << BLUE << left << setw(55) << title << RESET << " |\n";
        cout << "+" << string(58, '-') << "+\n";
    }

public:
    VotingSystem()
    {
        loadCandidates();
        loadVoterData();
    }

    ~VotingSystem()
    {
        saveCandidates();
        saveVoterData();
    }

    int getValidInput(const string &prompt, int min, int max)
    {
        int choice;
        while (true)
        {
            cout << prompt;
            cin >> choice;
            if (cin.fail() || choice < min || choice > max)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << RED << "Invalid input! Please enter between "
                     << min << "-" << max << RESET << "\n";
                continue;
            }
            break;
        }
        return choice;
    }

    void loadCandidates()
    {
        ifstream file(CANDIDATES_FILE);
        if (!file)
        {
            cout << YELLOW << "No candidates file found. New file will be created.\n"
                 << RESET;
            return;
        }

        candidates.clear();
        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string name, party, votesStr;

            try
            {
                getline(ss, name, '|');
                getline(ss, party, '|');
                getline(ss, votesStr);

                if (name.empty() || party.empty() || votesStr.empty())
                    throw invalid_argument("Invalid data format");

                candidates.push_back({name, party, stoi(votesStr)});
            }
            catch (...)
            {
                cout << RED << "Error loading candidate: " << line << RESET << "\n";
            }
        }
    }

    void saveCandidates()
    {
        ofstream file(CANDIDATES_FILE);
        if (!file)
        {
            cout << RED << "Failed to save candidates!\n"
                 << RESET;
            return;
        }

        for (const auto &c : candidates)
        {
            file << c.name << "|" << c.party << "|" << c.votes << "\n";
        }
    }

    void loadVoterData()
    {
        ifstream file(VOTERS_FILE);
        if (!file)
            return;

        votedVoters.clear();
        string voterID;
        while (getline(file, voterID))
        {
            if (!voterID.empty())
            {
                votedVoters.push_back(voterID);
            }
        }
    }

    void saveVoterData()
    {
        ofstream file(VOTERS_FILE);
        if (!file)
        {
            cout << RED << "Failed to save voter data!\n"
                 << RESET;
            return;
        }

        for (const auto &id : votedVoters)
        {
            file << id << "\n";
        }
    }

    void showCandidates()
    {
        drawTableHeader("CANDIDATE LIST");
        cout << "| " << left << setw(3) << "No."
             << "| " << setw(25) << "Name"
             << "| " << setw(25) << "Party" << " |\n";
        cout << "+" << string(58, '-') << "+\n";

        for (size_t i = 0; i < candidates.size(); ++i)
        {
            cout << "| " << setw(3) << i + 1
                 << "| " << setw(25) << candidates[i].name
                 << "| " << setw(25) << candidates[i].party << " |\n";
        }
        cout << "+" << string(58, '-') << "+\n\n";
    }

    void castVote(const string &voterID)
    {
        if (find(votedVoters.begin(), votedVoters.end(), voterID) != votedVoters.end())
        {
            cout << RED << "You have already voted!\n"
                 << RESET;
            return;
        }

        showCandidates();
        int choice = getValidInput("Enter candidate number: ", 1, candidates.size());

        candidates[choice - 1].votes++;
        votedVoters.push_back(voterID);
        cout << GREEN << "Vote recorded successfully!\n"
             << RESET;

        saveCandidates();
        saveVoterData();
    }

    void showResults()
    {
        if (candidates.empty())
        {
            cout << RED << "No candidates to show!\n"
                 << RESET;
            return;
        }

        vector<Candidate> sorted = candidates;
        sort(sorted.begin(), sorted.end(), [](const Candidate &a, const Candidate &b)
             { return a.votes > b.votes; });

        drawTableHeader("VOTING RESULTS");
        cout << "| " << left << setw(3) << "No."
             << "| " << setw(25) << "Name"
             << "| " << setw(15) << "Party"
             << "| " << setw(10) << "Votes" << " |\n";
        cout << "+" << string(58, '-') << "+\n";

        int totalVotes = 0;
        for (const auto &c : sorted)
            totalVotes += c.votes;

        for (size_t i = 0; i < sorted.size(); ++i)
        {
            cout << "| " << setw(3) << i + 1
                 << "| " << setw(25) << sorted[i].name
                 << "| " << setw(15) << sorted[i].party
                 << "| " << setw(10) << sorted[i].votes << " |\n";
        }
        cout << "+" << string(58, '-') << "+\n";

        if (totalVotes > 0)
        {
            cout << GREEN << "\nTotal votes cast: " << totalVotes << RESET << "\n";

            // Check for tie
            vector<Candidate> leaders;
            if (!sorted.empty() && sorted[0].votes > 0)
            {
                int maxVotes = sorted[0].votes;
                for (const auto &c : sorted)
                {
                    if (c.votes == maxVotes)
                        leaders.push_back(c);
                    else
                        break;
                }
            }

            if (!leaders.empty())
            {
                if (leaders.size() > 1)
                {
                    cout << YELLOW << "\nTIE BETWEEN " << leaders.size() << " CANDIDATES:\n"
                         << RESET;
                    for (const auto &c : leaders)
                    {
                        cout << YELLOW << "- " << c.name << " (" << c.party << ") "
                             << c.votes << " votes\n"
                             << RESET;
                    }
                }
                else
                {
                    cout << GREEN << "Leading candidate: " << sorted[0].name
                         << " (" << sorted[0].party << ") with "
                         << sorted[0].votes << " votes\n"
                         << RESET;
                }
            }
        }
        else
        {
            cout << YELLOW << "\nNo votes cast yet!\n"
                 << RESET;
        }
    }

    void adminMenu()
    {
        int choice;
        do
        {
            cout << "\n"
                 << BLUE << "ADMIN MENU\n"
                 << RESET;
            cout << "1. Add Candidate\n"
                 << "2. Delete Candidate\n"
                 << "3. View Results\n"
                 << "4. Reset System\n"
                 << "5. Return to Main Menu\n";

            choice = getValidInput("Enter choice: ", 1, 5);

            switch (choice)
            {
            case 1:
                addCandidate();
                break;
            case 2:
                deleteCandidate();
                break;
            case 3:
                showResults();
                break;
            case 4:
                resetSystem();
                break;
            }
        } while (choice != 5);
    }

    // Add this public method to allow access to adminPassword
    string getAdminPassword() const
    {
        return adminPassword;
    }

    void deleteCandidate()
    {
        if (candidates.empty())
        {
            cout << RED << "No candidates to delete!" << RESET << "\n";
            return;
        }
        showCandidates();
        int idx = getValidInput("Enter candidate number to delete: ", 1, candidates.size());

        // Confirmation prompt
        cout << YELLOW << "Are you sure you want to delete " << candidates[idx - 1].name
             << " (" << candidates[idx - 1].party << ")? (y/N): " << RESET;
        char confirm;
        cin >> confirm;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (tolower(confirm) != 'y')
        {
            cout << YELLOW << "Deletion cancelled.\n"
                 << RESET;
            return;
        }

        candidates.erase(candidates.begin() + (idx - 1));
        saveCandidates();
        cout << GREEN << "Candidate deleted successfully!" << RESET << "\n";
    }

    void resetSystem()
    {
        // Confirmation prompt
        cout << YELLOW << "Are you sure you want to reset the system? "
             << "This will delete ALL data! (y/N): " << RESET;
        char confirm;
        cin >> confirm;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (tolower(confirm) != 'y')
        {
            cout << YELLOW << "System reset cancelled.\n"
                 << RESET;
            return;
        }

        candidates.clear();
        votedVoters.clear();
        saveCandidates();
        saveVoterData();
        cout << GREEN << "System reset successfully!" << RESET << "\n";
    }

    void addCandidate()
    {
        string name, party;
        cout << "Enter candidate name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter party name: ";
        getline(cin, party);

        if (name.empty() || party.empty())
        {
            cout << RED << "Name and party cannot be empty!" << RESET << "\n";
            return;
        }

        // Check for existing candidate
        for (const auto &c : candidates)
        {
            if (c.name == name && c.party == party)
            {
                cout << RED << "Candidate already exists!\n"
                     << RESET;
                return;
            }
        }

        candidates.push_back({name, party, 0});
        saveCandidates();
        cout << GREEN << "Candidate added successfully!" << RESET << "\n";
    }
};

int main()
{
    VotingSystem vs;

    cout << "\n"
         << BLUE << "VOTING SYSTEM" << RESET << "\n";

    while (true)
    {
        cout << "\nMAIN MENU\n"
             << "1. Admin Login\n"
             << "2. Voter Login\n"
             << "3. Exit\n";

        int choice = vs.getValidInput("Enter choice: ", 1, 3);

        if (choice == 1)
        {
            string password;
            cout << "Enter admin password: ";
            cin >> password;

            if (password == vs.getAdminPassword())
            {
                vs.adminMenu();
            }
            else
            {
                cout << RED << "Incorrect password!\n"
                     << RESET;
            }
        }
        else if (choice == 2)
        {
            string voterID;
            cout << "Enter voter ID: ";
            cin >> voterID;
            vs.castVote(voterID);
        }
        else
        {
            cout << GREEN << "Exiting system...\n"
                 << RESET;
            break;
        }
    }
    return 0;
}