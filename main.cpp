#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include<algorithm>
using namespace std;

// Color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

class VotingSystem {
private:
    struct Candidate {
        string name;
        string party;
        int votes = 0;
    };

    vector<Candidate> candidates;

    vector<string> votedVoters;
    string adminPassword = "admin123";

public:
    void addDefaultCandidates() {
    candidates.push_back({" YOGI ADITYA NATH", "BJP", 0});

    candidates.push_back({"Nitish Kumar", "Coalition government", 0});

    candidates.push_back({"Uddhav Balasaheb Thackeray", "Shiv Sena", 0});

    candidates.push_back({" Ashok Gehlot", "Indian National Congress (INC)", 0});

    candidates.push_back({"Shri nayab Singh saini", "BJP", 0});
    }


    void showCandidates() {

        cout << "\n----------------------------------------\n";

        cout << "| No. |     Name       |     Party     |\n";

        cout << "----------------------------------------\n";

        for (size_t i = 0; i < candidates.size(); ++i) {

            cout << "| " << i + 1 << "   | ";

            cout.width(13); cout << left << candidates[i].name << "| ";

            cout.width(13); cout << left << candidates[i].party << "|\n";

            cout<<"\n";
        }

        cout << "----------------------------------------\n\n";
    }



    bool hasVoted(const string& voterID) {

        for (const auto& voter : votedVoters) {

            if (voter == voterID) {

                return true;
            }
        }
        return false;
    }

    void castVote(const string& voterID) {

        if (hasVoted(voterID)) {

            cout << RED << "You have already voted! Duplicate voting is not allowed.\n" << RESET;
            return;
        }

        showCandidates();
        cout << "Enter the number of the candidate you want to vote for: ";
        int choice;
        cin >> choice;

        if (choice < 1 || choice > static_cast<int>(candidates.size())) {

            cout << RED << "Invalid choice! Vote cancelled.\n" << RESET;

            return;
        }

        candidates[choice - 1].votes++;

        votedVoters.push_back(voterID);

        cout << GREEN << "✅ Your vote for " << candidates[choice - 1].name
             << " (" << candidates[choice - 1].party << ") has been recorded!\n" << RESET;
    }

 void showResults() {
    if (candidates.empty()) {
        cout << RED << "No candidates to show.\n" << RESET;
        return;
    }

    // Sort candidates by vote count in descending order
    vector<Candidate> sortedCandidates = candidates;
    sort(sortedCandidates.begin(), sortedCandidates.end(), [](const Candidate& a, const Candidate& b) {
        return a.votes > b.votes;
    });

    cout << "\n========== Voting Results ==========\n";
    cout << "| Rank |      Name       |    Party    | Votes |\n";
    cout << "-----------------------------------------\n";

    for (size_t i = 0; i < sortedCandidates.size(); ++i) {
        const auto& c = sortedCandidates[i];
        cout << "|  " << i + 1 << "   | ";

        cout.width(15); cout << left << c.name << "| ";
        cout.width(11); cout << left << c.party << "| ";
        cout << c.votes << "   |\n";
    }

    cout << "====================================\n";

    // Highlight winner
    const auto& winner = sortedCandidates.front();
    cout << GREEN << "\n🏆 Winner: " << winner.name << " (" << winner.party
         << ") with " << winner.votes << " votes!\n" << RESET;
}

    void addCandidate() {

        string name;

        string party;

        cout << "Enter the name of the new candidate: ";

        cin.ignore();  // clear input buffer

        getline(cin, name);

        cout << "Enter the party of the new candidate: ";

        getline(cin, party);

        candidates.push_back({name, party, 0});

        cout << GREEN << "Candidate added successfully.\n" << RESET;
    }

    void countCandidates() {
        cout << "\n===== Candidate Details =====\n";

        cout << "Total number of candidates: " << candidates.size() << "\n\n";

        for (size_t i = 0; i < candidates.size(); ++i) {

            cout << i + 1 << ". Name: " << candidates[i].name
                 << ", Party: " << candidates[i].party << "\n";
        }
        cout << "==============================\n\n";
    }

    void deleteCandidate() {
        if (candidates.empty()) {

            cout << RED << "No candidates to delete.\n" << RESET;

            return;
        }
        showCandidates();

        cout << "Enter the number of the candidate you want to delete: ";

        int choice;

        cin >> choice;

        if (choice < 1 || choice > static_cast<int>(candidates.size())) {

            cout << RED << "Invalid choice! Deletion cancelled.\n" << RESET;

            return;
        }

        cout << GREEN << "Candidate " << candidates[choice - 1].name
             << " (" << candidates[choice - 1].party << ") has been deleted.\n" << RESET;
        candidates.erase(candidates.begin() + (choice - 1));
    }

    void showVoters() {
        if (votedVoters.empty()) {
            cout << RED << "No voters have voted yet.\n" << RESET;
            return;
        }
        cout << "\n===== List of Voters =====\n";
        for (size_t i = 0; i < votedVoters.size(); ++i) {
            cout << i + 1 << ". Voter ID: " << votedVoters[i] << "\n";
        }
        cout << "===========================\n\n";
    }

    void adminLogin() {
        system("cls");
        string password;
        cout << "Enter Admin Password: ";
        cin >> password;
        if (password == adminPassword) {
            int choice;
            do {
                cout << "\n--- Admin Menu ---\n";

                cout << "1. Add Candidate\n";

                cout << "2. Delete Candidate\n";

                cout << "3. View Results\n";

                cout << "4. View Candidates Count and Details\n";

                cout << "5. Show Voters\n";

                cout << "6. Logout\n";

                cout << "Enter your choice: ";

                cin >> choice;

                switch (choice) {

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

                        countCandidates();

                        break;
                    case 5:

                        showVoters();

                        break;

                    case 6:

                        cout << GREEN << "Logging out...\n" << RESET;

                        break;

                    default:

                        cout << RED << "Invalid choice!\n" << RESET;
                }
            } while (choice != 6);

        } else {

            cout << RED << "Wrong password! Access denied.\n" << RESET;
        }
    }

    void voterLogin() {

        string voterID;

        cout << "Enter your Voter ID (must be unique): ";

        cin >> voterID;

        castVote(voterID);
    }
};

int main() {
    VotingSystem vs;

    vs.addDefaultCandidates();

cout << "\n";
cout << "*                                      *\n";
cout << "*                                      *\n";
cout << "*             WELCOME                  *\n";
cout << "*          TO VOTING SYSTEM            *\n";
cout << "*                                      *\n";
cout << "*                                      *\n";
cout << "\n";



    int option;
    do {

        cout << "\nMain Menu:\n";

        cout << "1. Admin Login\n";

        cout << "2. Voter Login\n";

        cout << "3. Exit\n";

        cout << "Choose an option: ";

        cin >> option;
        switch (option) {

            case 1:

                vs.adminLogin();
                break;
            case 2:

                vs.voterLogin();
                break;
            case 3:

                cout << GREEN << "Thank you for using the Voting System. Goodbye!\n" << RESET;

                break;
            default:

                cout << RED << "Invalid choice! Please try again.\n" << RESET;
        }
    } while (option != 3);

    return 0;
}