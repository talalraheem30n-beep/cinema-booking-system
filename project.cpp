#include <iostream>
#include <string>
using namespace std;
// Base class using inheritance
class Item {
private:
    string name;

public:
    Item(string n) {
        name = n;
    }
    string getName() const {
        return name;
    }
    void setName(string n) {
        name = n;
    }
    virtual void showInfo() = 0;
    virtual ~Item() {}
};
class SeatMap {
private:
    char* seats;
    int SIZE;
    int TOTAL_SEATS;

public:
    SeatMap() {
        SIZE = 5;
        TOTAL_SEATS = 25;

        seats = new char[TOTAL_SEATS];
        for (int i = 0; i < TOTAL_SEATS; i++) {
            seats[i] = '0';
        }
    }

    SeatMap(const SeatMap& other) {
        SIZE = other.SIZE;
        TOTAL_SEATS = other.TOTAL_SEATS;

        seats = new char[TOTAL_SEATS];
        for (int i = 0; i < TOTAL_SEATS; i++) {
            seats[i] = other.seats[i];
        }
    }
    //Destructor
    ~SeatMap() {
        delete[] seats;
    }

    int getSize() const { return SIZE; }
    int getTotalSeats() const { return TOTAL_SEATS; }

    int getIndex(int r, int c) {
        return r * SIZE + c;
    }

    void displaySeats() {
        cout << "\n   1 2 3 4 5";
        for (int i = 0; i < SIZE; i++) {
            cout << "\n" << i + 1 << "  ";
            for (int j = 0; j < SIZE; j++) {
                cout << seats[getIndex(i, j)] << " ";
            }
        }
        cout << endl;
    }
    bool isSeatAvailable(int r, int c) {
        return seats[getIndex(r, c)] == '0';
    }

    void book(int r, int c) {
        seats[getIndex(r, c)] = 'X';
    }
};
// Movie class
class Movie : public Item {
private:
    int price;
    SeatMap seatmap;

public:
    Movie(string n, int p) : Item(n) {
        price = p;
    }

    int getPrice() const { return price; }
    void setPrice(int p) { price = p; }
    SeatMap& getSeatMap() { return seatmap; }

    void showInfo() {
        cout << "\nMovie: " << getName();
        cout << "\nTicket Price: " << price << "\n";
        seatmap.displaySeats();
    }

    ~Movie() {}
};

// BookingSystem class
class BookingSystem {
private:
    Movie* movies[3];
    int totalRevenue;

public:
    BookingSystem() {
        movies[0] = new Movie("Avengers", 700);
        movies[1] = new Movie("IT", 650);
        movies[2] = new Movie("Joker", 600);
        totalRevenue = 0;
    }

    ~BookingSystem() {
        for (int i = 0; i < 3; i++) {
            delete movies[i];
        }
    }

    void showMovies() {
        cout << "\nMOVIES LIST\n";
        for (int i = 0; i < 3; i++) {
            cout << i + 1 << ". " << movies[i]->getName()
                << " (Rs " << movies[i]->getPrice() << ")\n";
        }
    }

    bool processPayment(int amount) {
        int choice;
        cout << "\nSelect Payment Method:";
        cout << "\n1. Cash\n2. Card\n";
        cout << "Enter choice : ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input.\n";
            return false;
        }

        if (choice == 1) {
            int cash;
            cout << "Enter cash amount: ";
            cin >> cash;
            if (cash < amount) {
                cout << "Insufficient funds.\n";
                return false;
            }
            cout << "Payment successful. Change: Rs " << cash - amount << endl;
            return true;
        }
        else if (choice == 2) {
            string cardNum;
            cout << "Enter 4-digit card number: ";
            cin >> cardNum;

            if (cardNum.length() != 4) {
                cout << "Invalid card.\n";
                return false;
            }

            cout << "Card payment approved.\n";
            return true;
        }

        cout << "Invalid payment type.\n";
        return false;
    }
    void bookTicket(int movieIndex, int (*discountFunction)(int)) {
        Movie* m = movies[movieIndex];
        m->showInfo();

        int seatCount;
        cout << "How many seats do you want to book (1-5)? ";
        cin >> seatCount;

        if (seatCount < 1 || seatCount > 5) {
            cout << "Invalid number of seats.\n";
            return;
        }
        int rows[5], cols[5];
        for (int i = 0; i < seatCount; i++) {
            bool ok = false;

            while (!ok) {
                cout << "\nSeat " << i + 1 << ":\n";
                int r, c;
                cout << "Enter row (1-5): ";
                cin >> r;
                cout << "Enter column (1-5): ";
                cin >> c;
                if (r < 1 || r > 5 || c < 1 || c > 5) {
                    cout << "Invalid seat.\n";
                    continue;
                }
                r--; c--;
                bool duplicate = false;
                for (int k = 0; k < i; k++) {
                    if (rows[k] == r && cols[k] == c) {
                        cout << "You selected this seat already.\n";
                        duplicate = true;
                        break;
                    }
                }
                if (duplicate) continue;

                if (!m->getSeatMap().isSeatAvailable(r, c)) {
                    cout << "Seat already booked.\n";
                    continue;
                }

                rows[i] = r;
                cols[i] = c;
                ok = true;
            }
        }
        int amount = seatCount * m->getPrice();
        char wantDiscount;
        cout << "\nOriginal amount: Rs " << amount;
        cout << "\nApply 10% discount (y/n)? ";
        cin >> wantDiscount;
        if (wantDiscount == 'y' || wantDiscount == 'Y') {
            amount = discountFunction(amount);
            cout << "After discount: Rs " << amount << endl;
        }
        if (processPayment(amount)) {
            for (int i = 0; i < seatCount; i++) {
                m->getSeatMap().book(rows[i], cols[i]);
            }
            totalRevenue += amount;
            cout << "Booking successful.\n";
        }
        else {
            cout << "Payment failed.\n";
        }
    }
    void showRevenue() {
        cout << "\nTotal Revenue: Rs " << totalRevenue << endl;
    }
};
//Function as argument
int applyDiscount(int amt) {
    return amt - (amt / 10);
}
//Main body
int main() {
    BookingSystem system;
    int choice;
    while (true) {
        cout << "\nCinema Booking System";
        cout << "\n1. Show Movies";
        cout << "\n2. Book Ticket";
        cout << "\n3. Show Revenue";
        cout << "\n4. Exit";
        cout << "\nEnter choice: ";
        cin >> choice;
        if (choice == 1) system.showMovies();
        else if (choice == 2) {
            int mov;
            system.showMovies();
            cout << "Select movie (1-3): ";
            cin >> mov;
            if (mov >= 1 && mov <= 3)
                system.bookTicket(mov - 1, applyDiscount);
            else
                cout << "Invalid selection.\n";
        }
        else if (choice == 3) system.showRevenue();
        else if (choice == 4) {
            cout << "Goodbye.\n";
            break;
        }
        else {
            cout << "Invalid choice.\n";
        }
    }
}