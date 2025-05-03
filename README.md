# Voting System

A console-based voting application with administrative controls, persistent data storage, and secure voting functionality.

## Table of Contents
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
  - [Admin Access](#admin-access)
  - [Voter Access](#voter-access)
- [File Storage](#file-storage)
- [Contributing](#contributing)
- [License](#license)

## Features

- **User-friendly Interface**: Simple console-based menu system with colorful outputs
- **Multiple Access Levels**: 
  - Admin mode for system management
  - Voter mode for casting votes
- **Admin Functionality**:
  - Add/delete candidates
  - View real-time voting results 
  - Reset the entire voting system
- **Voter Functionality**:
  - Single vote per voter ID (duplicate prevention)
  - Clear display of candidate options
- **Data Persistence**:
  - Automatic saving of candidates and votes
  - Automatic loading of previous voting data on startup
- **Security Features**:
  - Password-protected admin access
  - Prevention of duplicate voting
- **Results Handling**:
  - Detailed vote counting
  - Automatic tie detection
  - Sorted results display


## Installation
```bash
git clone https://github.com/Cipherex/Voting-management-system

```
```bash
cd Voting-management-system
```

### Prerequisites
- C++ compiler (GCC, Clang, MSVC, etc.)
- Standard C++ libraries

### Compiling the Code

#### Using G++:
```bash
g++ -o main.cpp
```

#### Using Visual Studio:
1. Create a new C++ project
2. Add the main.cpp file to your project
3. Build the solution

## Usage

### Running the Application
```bash
./main
```

The application starts with a main menu offering three options:
1. Admin Login
2. Voter Login
3. Exit

### Admin Access
- Default password: `admin123`
- Admin Menu Options:
  1. Add Candidate
  2. Delete Candidate
  3. View Results
  4. Reset Voting System
  5. Return to main menu

### Voter Access
- Requires unique Voter ID for each vote
- Prevents duplicate voting
- Presents a list of candidates to choose from

## File Storage

The system maintains three data files:
- `candidates.txt`: Stores candidate information and vote counts
- `voters.txt`: Keeps a record of voter IDs who have already voted

Data format in `candidates.txt`:
```
CandidateName|Party|VoteCount
```

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

---
