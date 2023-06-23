*******************************************************************************/
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <numeric>

class Passenger;
class TicketBooker;

void bookTicket(Passenger p);
void cancelTicket(int id);

class Passenger {
public:
    static int id;
    std::string name;
    int age;
    std::string berthPreference;
    int passengerId;
    std::string alloted;
    int number;
    
    Passenger() = default;
    Passenger(std::string name, int age, std::string berthPreference)
        : name(name), age(age), berthPreference(berthPreference), passengerId(id++), alloted(""), number(-1) {}
};

class TicketBooker {
public:
    static int availableLowerBerths;
    static int availableMiddleBerths;
    static int availableUpperBerths;
    static int availableRacTickets;
    static int availableWaitingList;

    static std::queue<int> waitingList;
    static std::queue<int> racList;

    static std::vector<int> bookedTicketList;

    static std::vector<int>lowerBerthsPositions;
    
    static std::vector<int> middleBerthsPositions;
    static std::vector<int> upperBerthsPositions;

    static std::vector<int> racPositions;
    static std::vector<int> waitingListPositions;

    static std::map<int, Passenger> passengers;
    TicketBooker() {
        for (int i = 0; i < 10; i++) {
        lowerBerthsPositions[i] = i + 1; 
    }

    }
    
   
    void bookTicket(Passenger p, int berthInfo, std::string allotedBerth) {
        p.number = berthInfo;
        p.alloted = allotedBerth;
        passengers[p.passengerId] = p;
        bookedTicketList.push_back(p.passengerId);
        std::cout << "--------------------------Booked Successfully" << std::endl;
    }

    void addToRac(Passenger p, int racInfo, std::string allotedrac) {
        p.number = racInfo;
        p.alloted = allotedrac;
        passengers[p.passengerId] = p;
        racList.push(p.passengerId);
        availableRacTickets--;
        racPositions.erase(racPositions.begin());
        std::cout << "------------added to RAC Successfully" << std::endl;
    }

    void addToWaitingList(Passenger p, int wtInfo, std::string allotedWL) {
        p.number = wtInfo;
        p.alloted = allotedWL;
        passengers[p.passengerId] = p;
        waitingList.push(p.passengerId);
        availableWaitingList--;
        waitingListPositions.erase(waitingListPositions.begin());
        std::cout << "------------added to waiting list Successfully" << std::endl;
    }

    void cancel(int id) {
        Passenger p = passengers[id];
        passengers.erase(id);
        bookedTicketList.erase(std::remove(bookedTicketList.begin(), bookedTicketList.end(), id), bookedTicketList.end());

        int positionbooked = p.number;
        std::cout << "------------------cancelled Successfully" << std::endl;
        if (p.alloted == "L") {
            availableLowerBerths++;
            lowerBerthsPositions.push_back(positionbooked);
        } else if (p.alloted == "M") {
            availableMiddleBerths++;
            middleBerthsPositions.push_back(positionbooked);
        } else if (p.alloted == "U") {
            availableUpperBerths++;
            upperBerthsPositions.push_back(positionbooked);
        }

        if (racList.size() > 0) {
            Passenger prac = passengers[racList.front()];
            int position = prac.number;
            racPositions.push_back(position);
            racList.pop();
            availableRacTickets++;

            if (waitingList.size() > 0) {
                Passenger pw = passengers[waitingList.front()];
                int position = pw.number;
                waitingListPositions.push_back(position);
                waitingList.pop();
                availableWaitingList++;

                pw.number = racPositions[0];
                pw.alloted = "RAC";
                racPositions.erase(racPositions.begin());
                racList.push(pw.passengerId);
                availableRacTickets--;
            }
            bookTicket(prac, prac.number, prac.alloted);
        }
    }

    void printAvailable() {
        std::cout << "Available Lower Berth: " << availableLowerBerths << std::endl;
        std::cout << "Available Middle Berth: " << availableMiddleBerths << std::endl;
        std::cout << "Available Upper Berth: " << availableUpperBerths << std::endl;
        std::cout << "Available RAC: " << availableRacTickets << std::endl;
        std::cout << "Available Waiting List: " << availableWaitingList << std::endl;
    }

    void printPassengers() {
        std::cout<<passengers.size()<<std::endl;
        if (passengers.size() == 0) {
            std::cout << "No details of passengers" << std::endl;
            return;
        }
        for (const auto& i : passengers) {
            Passenger p = i.second;
            std::cout << "Passenger ID: " << p.passengerId << std::endl;
            std::cout << "Name: " << p.name << std::endl;
            std::cout << "Age: " << p.age << std::endl;
            std::cout << "Status: " << p.number << p.alloted << std::endl;
        }
    }
};

int Passenger::id = 1;
int TicketBooker::availableLowerBerths = 10;
int TicketBooker::availableMiddleBerths = 1;
int TicketBooker::availableUpperBerths = 1;
int TicketBooker::availableRacTickets = 1;
int TicketBooker::availableWaitingList = 1;

std::queue<int> TicketBooker::waitingList;
std::queue<int> TicketBooker::racList;

std::vector<int> TicketBooker::bookedTicketList;

std::vector<int> TicketBooker::lowerBerthsPositions(10);

std::vector<int> TicketBooker::middleBerthsPositions(1);
std::vector<int> TicketBooker::upperBerthsPositions(1);

std::vector<int> TicketBooker::racPositions(1);
std::vector<int> TicketBooker::waitingListPositions(1);
std::map<int, Passenger>TicketBooker::passengers;
void bookTicket(Passenger p) {
    TicketBooker booker;
    if (booker.availableWaitingList == 0) {
        std::cout << "No Tickets Available" << std::endl;
        return;
    }

    if ((p.berthPreference == "L" && booker.availableLowerBerths > 0) ||
        (p.berthPreference == "M" && booker.availableMiddleBerths > 0) ||
        (p.berthPreference == "U" && booker.availableUpperBerths > 0)) {
        std::cout << "Preferred Berth Available" << std::endl;
        if (p.berthPreference == "L") {
            std::cout << "Lower Berth Given" << std::endl;
            booker.bookTicket(p, TicketBooker::lowerBerthsPositions.front(), "L");
            TicketBooker::lowerBerthsPositions.erase(TicketBooker::lowerBerthsPositions.begin());
            TicketBooker::availableLowerBerths--;
        } else if (p.berthPreference == "M") {
            std::cout << "Middle Berth Given" << std::endl;
            booker.bookTicket(p, TicketBooker::middleBerthsPositions.front(), "M");
            TicketBooker::middleBerthsPositions.erase(TicketBooker::middleBerthsPositions.begin());
            TicketBooker::availableMiddleBerths--;
        } else if (p.berthPreference == "U") {
            std::cout << "Upper Berth Given" << std::endl;
            booker.bookTicket(p, TicketBooker::upperBerthsPositions.front(), "U");
            TicketBooker::upperBerthsPositions.erase(TicketBooker::upperBerthsPositions.begin());
            TicketBooker::availableUpperBerths--;
        }
    } else if (booker.availableLowerBerths > 0) {
        std::cout << "Preferred berth not available, Lower Berth Given" << std::endl;
        booker.bookTicket(p, TicketBooker::lowerBerthsPositions.front(), "L");
        TicketBooker::lowerBerthsPositions.erase(TicketBooker::lowerBerthsPositions.begin());
        TicketBooker::availableLowerBerths--;
    } else if (booker.availableMiddleBerths > 0) {
        std::cout << "Preferred berth not available, Middle Berth Given" << std::endl;
        booker.bookTicket(p, TicketBooker::middleBerthsPositions.front(), "M");
        TicketBooker::middleBerthsPositions.erase(TicketBooker::middleBerthsPositions.begin());
        TicketBooker::availableMiddleBerths--;
    } else if (booker.availableUpperBerths > 0) {
        std::cout << "Preferred berth not available, Upper Berth Given" << std::endl;
        booker.bookTicket(p, TicketBooker::upperBerthsPositions.front(), "U");
        TicketBooker::upperBerthsPositions.erase(TicketBooker::upperBerthsPositions.begin());
        TicketBooker::availableUpperBerths--;
    } else if (booker.availableRacTickets > 0) {
        std::cout << "RAC Available" << std::endl;
        booker.addToRac(p, TicketBooker::racPositions.front(), "RAC");
    } else if (booker.availableWaitingList > 0) {
        std::cout << "Waiting List Available" << std::endl;
        booker.addToWaitingList(p, TicketBooker::waitingListPositions.front(), "WL");
    }
}

void cancelTicket(int id) {
    TicketBooker booker;

    if (booker.passengers.find(id) == booker.passengers.end()) {
        std::cout << "Passenger detail unknown" << std::endl;
    } else {
        booker.cancel(id);
    }
}

int main() {
    bool loop = true;

    while (loop) {
        std::cout << " 1. Book Ticket \n 2. Cancel Ticket \n 3. Available Tickets \n 4. Booked Tickets \n 5. Exit" << std::endl;
        int choice;
        std::cin >> choice;

        switch (choice) {
            // Book ticket
            case 1: {
                std::cout << "Enter Passenger name, age, and berth preference (L, M, or U): ";
                std::string name;
                int age;
                std::string berthPreference;
                std::cin >> name >> age >> berthPreference;

                Passenger p(name, age, berthPreference);
                bookTicket(p);
                break;
            }
            // Cancel ticket
            case 2: {
                std::cout << "Enter Passenger ID to cancel: ";
                int id;
                std::cin >> id;
                cancelTicket(id);
                break;
            }
            // Available tickets
            case 3: {
                TicketBooker booker;
                booker.printAvailable();
                break;
            }
            // Booked tickets
            case 4: {
                TicketBooker booker;
                booker.printPassengers();
                break;
            }
            // Exit
            case 5: {
                loop = false;
                break;
            }
            default:
                std::cout << "Invalid choice" << std::endl;
        }
    }

    return 0;
}
