#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;

// Color codes - fixed to use standard escape sequences
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

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
    
    // File paths for storing data
    const string CANDIDATES_FILE = "candidates.txt";
    const string VOTES_FILE = "votes.txt";
    const string VOTERS_FILE = "voters.txt";

public:
    // Constructor now loads data from files
    VotingSystem() {
        loadCandidates();
        loadVoterData();
    }
    
    // Destructor to ensure data is saved when program exits
    ~VotingSystem() {
        saveCandidates();
        saveVoterData();
    }

    void addDefaultCandidates() {
        // Only add default candidates if there are none loaded
        if (candidates.empty()) {
            candidates.push_back({"YOGI ADITYA NATH", "BJP", 0});
            candidates.push_back({"Nitish Kumar", "Coalition Govt", 0});
            candidates.push_back({"Uddhav Thackeray", "Shiv Sena", 0});
            candidates.push_back({"Ashok Gehlot", "INC", 0});
            candidates.push_back({"Nayab Singh Saini", "TMC", 0});
            
            // Save the default candidates to file
            saveCandidates();
            cout << BLUE << "Default candidates have been added.\n" << RESET;
        }
    }

    // File I/O Functions
    void loadCandidates() {
        ifstream file(CANDIDATES_FILE);
        
        if (!file.is_open()) {
            cout << YELLOW << "No existing candidates file found. Will create a new one when needed.\n" << RESET;
            return;
        }
        
        string line;
        candidates.clear();
        
        while (getline(file, line)) {
            stringstream ss(line);
            string name, party, votesStr;
            
            // Format: name|party|votes
            getline(ss, name, '|');
            getline(ss, party, '|');
            getline(ss, votesStr, '|');
            
            int votes = stoi(votesStr);
            candidates.push_back({name, party, votes});
        }
        
        file.close();
        cout << BLUE << "Candidates and vote counts loaded from file.\n" << RESET;
    }
    
    void saveCandidates() {
        ofstream file(CANDIDATES_FILE);
        
        if (!file.is_open()) {
            cout << RED << "Error: Could not open candidates file for writing.\n" << RESET;
            return;
        }
        
        for (const auto& c : candidates) {
            file << c.name << "|" << c.party << "|" << c.votes << endl;
        }
        
        file.close();
        cout << BLUE << "Candidates and vote counts saved to file.\n" << RESET;
    }
    
    void loadVoterData() {
        ifstream file(VOTERS_FILE);
        
        if (!file.is_open()) {
            cout << YELLOW << "No existing voters file found. Will create a new one when needed.\n" << RESET;
            return;
        }
        
        string voterID;
        votedVoters.clear();
        
        while (getline(file, voterID)) {
            votedVoters.push_back(voterID);
        }
        
        file.close();
        cout << BLUE << "Voter data loaded from file.\n" << RESET;
    }
    
    void saveVoterData() {
        ofstream file(VOTERS_FILE);
        
        if (!file.is_open()) {
            cout << RED << "Error: Could not open voters file for writing.\n" << RESET;
            return;
        }
        
        for (const auto& voterID : votedVoters) {
            file << voterID << endl;
        }
        
        file.close();
        cout << BLUE << "Voter data saved to file.\n" << RESET;
    }

    void showCandidates() {
        cout << "\n+------+----------------------+----------------------+\n";
        cout << "| No.  |        Name          |        Party        |\n";
        cout << "+------+----------------------+----------------------+\n";

        for (size_t i = 0; i < candidates.size(); ++i) {
            cout << "| " << i + 1;
            if (i + 1 < 10) cout << "    ";
            else cout << "   ";
            
            cout << "| ";
            cout.width(20); cout << left << candidates[i].name << "| ";
            cout.width(20); cout << left << candidates[i].party << "|\n";
            cout << "+------+----------------------+----------------------+\n";
        }
        cout << "\n";
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

        cout << GREEN << "Your vote for " << candidates[choice - 1].name
             << " (" << candidates[choice - 1].party << ") has been recorded!\n" << RESET;
        
        // Save the updated data immediately
        saveCandidates();
        saveVoterData();
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

        cout << "\n+======================================================+\n";
        cout << "|                  VOTING RESULTS                      |\n";
        cout << "+------+----------------------+----------------+--------+\n";
        cout << "| Rank |        Name          |     Party     | Votes  |\n";
        cout << "+------+----------------------+----------------+--------+\n";

        for (size_t i = 0; i < sortedCandidates.size(); ++i) {
            const auto& c = sortedCandidates[i];
            cout << "|  " << i + 1;
            if (i + 1 < 10) cout << "   ";
            else cout << "  ";
            
            cout << "| ";
            cout.width(20); cout << left << c.name << "| ";
            cout.width(14); cout << left << c.party << "| ";
            cout.width(6); cout << right << c.votes << " |\n";
            cout << "+------+----------------------+----------------+--------+\n";
        }

        // Check for ties and handle them
        if (sortedCandidates.size() >= 2 && sortedCandidates[0].votes == sortedCandidates[1].votes && sortedCandidates[0].votes > 0) {
            // Create a list of candidates with the highest vote count
            vector<Candidate> tiedCandidates;
            int highestVotes = sortedCandidates[0].votes;
            
            for (const auto& c : sortedCandidates) {
                if (c.votes == highestVotes) {
                    tiedCandidates.push_back(c);
                } else {
                    break; // Only collect candidates with the highest votes
                }
            }
            
            cout << "\n" << GREEN << "*** TIE DETECTED! *** The following candidates have tied with " 
                 << highestVotes << " votes each:\n" << RESET;
                 
            for (const auto& c : tiedCandidates) {
                cout << GREEN << "- " << c.name << " (" << c.party << ")\n" << RESET;
            }
            
            cout << GREEN << "A runoff election may be needed to determine the winner.\n" << RESET;
        } else if (!sortedCandidates.empty() && sortedCandidates[0].votes > 0) {
            // No tie - announce single winner
            const auto& winner = sortedCandidates.front();
            cout << GREEN << "\nWinner: " << winner.name << " (" << winner.party
                 << ") with " << winner.votes << " votes!\n" << RESET;
        } else {
            cout << YELLOW << "\nNo votes have been cast yet.\n" << RESET;
        }
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
        
        // Save immediately after adding a candidate
        saveCandidates();
    }

    void countCandidates() {
        cout << "\n+=============================================+\n";
        cout << "|             CANDIDATE DETAILS              |\n";
        cout << "+=============================================+\n";
        cout << "| Total number of candidates: " << candidates.size() << "\n\n";

        for (size_t i = 0; i < candidates.size(); ++i) {
            cout << "| " << i + 1 << ". Name: " << candidates[i].name
                 << ", Party: " << candidates[i].party << "\n";
        }
        cout << "+=============================================+\n\n";
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
        
        // Save immediately after deleting a candidate
        saveCandidates();
    }

    void showVoters() {
        if (votedVoters.empty()) {
            cout << RED << "No voters have voted yet.\n" << RESET;
            return;
        }
        
        cout << "\n+=============================================+\n";
        cout << "|               LIST OF VOTERS               |\n";
        cout << "+-----+------------------------------------+\n";
        cout << "| No. |              Voter ID              |\n";
        cout << "+-----+------------------------------------+\n";
        
        for (size_t i = 0; i < votedVoters.size(); ++i) {
            cout << "| " << i + 1;
            if (i + 1 < 10) cout << "   ";
            else cout << "  ";
            
            cout << "| " << votedVoters[i];
            // Pad to align with table width
            int padding = 34 - votedVoters[i].length();
            for (int j = 0; j < padding; j++) {
                cout << " ";
            }
            cout << "|\n";
            cout << "+-----+------------------------------------+\n";
        }
        cout << "\n";
    }
    
    void resetSystem() {
        string confirmation;
        cout << RED << "WARNING: This will reset all votes and clear all voter records.\n";
        cout << "Type 'CONFIRM' to proceed or anything else to cancel: " << RESET;
        cin >> confirmation;
        
        if (confirmation == "CONFIRM") {
            // Reset all vote counts to 0
            for (auto& candidate : candidates) {
                candidate.votes = 0;
            }
            
            // Clear voter list
            votedVoters.clear();
            
            // Save the reset data
            saveCandidates();
            saveVoterData();
            
            cout << GREEN << "Voting system has been reset. All votes cleared.\n" << RESET;
        } else {
            cout << BLUE << "Reset operation cancelled.\n" << RESET;
        }
    }

    void adminLogin() {
        #ifdef _WIN32
            system("cls");  // Windows
        #else
            system("clear"); // Unix/Linux
        #endif
        
        string password;
        cout << "Enter Admin Password: ";
        cin >> password;
        
        if (password == adminPassword) {
            int choice;
            do {
                cout << "\n+------------------------------+\n";
                cout << "|         ADMIN MENU          |\n";
                cout << "+------------------------------+\n";
                cout << "| 1. Add Candidate            |\n";
                cout << "| 2. Delete Candidate         |\n";
                cout << "| 3. View Results             |\n";
                cout << "| 4. View Candidates Details  |\n";
                cout << "| 5. Show Voters              |\n";
                cout << "| 6. Reset Voting System      |\n";
                cout << "| 7. Save Data to Files       |\n";
                cout << "| 8. Load Data from Files     |\n";
                cout << "| 9. Logout                   |\n";
                cout << "+------------------------------+\n";
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
                        resetSystem();
                        break;
                    case 7:
                        saveCandidates();
                        saveVoterData();
                        break;
                    case 8:
                        loadCandidates();
                        loadVoterData();
                        break;
                    case 9:
                        cout << GREEN << "Logging out...\n" << RESET;
                        break;
                    default:
                        cout << RED << "Invalid choice!\n" << RESET;
                }
            } while (choice != 9);
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
    
    cout << "\n";
    cout << "+--------------------------------------+\n";
    cout << "|                                      |\n";
    cout << "|               WELCOME                |\n";
    cout << "|            VOTING SYSTEM             |\n";
    cout << "|                                      |\n";
    cout << "+--------------------------------------+\n";
    cout << "\n";
    
    // Add default candidates only if there are no candidates loaded from file
    if (vs.addDefaultCandidates(), true) {}

    int option;
    do {
        cout << "\n+------------------------------+\n";
        cout << "|           MAIN MENU          |\n";
        cout << "+------------------------------+\n";
        cout << "| 1. Admin Login              |\n";
        cout << "| 2. Voter Login              |\n";
        cout << "| 3. Exit                     |\n";
        cout << "+------------------------------+\n";
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