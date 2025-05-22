# Family Expense Management System

This practical implements a simple family expense management system in both Ruby and Smalltalk. The system allows for creating families with up to two members (either teenagers or adults) and manages their monthly expenses and savings.

## Ruby Implementation

### Requirements
- Ruby (any recent version)

### How to Run
1. Navigate to the ruby directory:
   ```bash
   cd ruby
   ```
2. Run the program:
   ```bash
   ruby main.rb
   ```

## Smalltalk Implementation

### Requirements
- GNU Smalltalk

### Installing GNU Smalltalk

#### On Ubuntu/Debian:
There are two ways to install GNU Smalltalk:

1. Using package manager (for Ubuntu 22.04 and earlier):
```bash
sudo apt-get update
sudo apt-get install gnu-smalltalk
```

2. Building from source (for Ubuntu 24.04 and newer):
```bash
# Install build dependencies
sudo apt-get update
sudo apt-get install -y build-essential libsigsegv-dev libffi-dev libltdl-dev libgmp-dev pkg-config zip

# Download and extract source
cd /tmp
wget https://ftp.gnu.org/gnu/smalltalk/smalltalk-3.2.5.tar.gz
tar xzf smalltalk-3.2.5.tar.gz
cd smalltalk-3.2.5

# Configure and build
./configure
make
sudo make install

# Update shared library cache
sudo ldconfig
```

#### On Fedora:
```bash
sudo dnf install gnu-smalltalk
```

#### On macOS (using Homebrew):
```bash
brew install gnu-smalltalk
```

#### Verifying Installation:
After installation, verify it works by running:
```bash
gst --version
```

### How to Run
1. Navigate to the smalltalk directory:
   ```bash
   cd smalltalk
   ```
2. Run the program:
   ```bash
   gst family.st
   ```

## Program Usage

The program will:
1. Prompt you to add two family members (either teenagers or adults)
2. For teenagers, you'll need to enter:
   - Monthly pocket money
   - Monthly expenses
3. For adults, you'll need to enter:
   - Monthly salary
   - Interest rate (as a percentage)
   - Monthly expenses
4. After adding members, you can simulate multiple months by entering 'y' when prompted
5. The program will show the savings for each family member after each month
6. Enter 'n' to end the simulation and see final savings

## Implementation Details

Both implementations follow object-oriented principles with:
- Abstract base class `FamilyMember`
- Two concrete classes: `Teenager` and `Adult`
- A `Family` class to manage family members

The main differences between Ruby and Smalltalk implementations are:
- Syntax and language-specific constructs
- Method naming conventions
- Instance variable access patterns
- Class definition structure 
