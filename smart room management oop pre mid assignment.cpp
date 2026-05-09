#include <iostream>  
#include <fstream>    
#include <string>     
using namespace std;

// Device class represents a smart device
class Device {
private:
    string name;      // device name
    bool isOn;        // ON/OFF status
    int brightness;   // brightness level (0–100)

public:
    // Default constructor
    Device() {
        name = "Unknown";
        isOn = false;
        brightness = 50; // default brightness
    }

    // Parameterized constructor
    Device(string n, bool status, int b) {
        name = n;
        isOn = status;
        setBrightness(b); 
    }

    // Destructor
    ~Device() {
        cout << "Device [" << name << "] is being removed.\n";
    }

    // Set device name
    void setName(string n) {
        name = n;
    }

    // Get device name
    string getName() const {
        return name;
    }

    // Set brightness with validation
    void setBrightness(int b) {
        if (b >= 0 && b <= 100)
            brightness = b;
        else
            brightness = 50; 
    }

    // Get brightness
    int getBrightness() const {
        return brightness;
    }

    // Turn device ON
    void turnOn() {
        isOn = true;
    }

    // Turn device OFF
    void turnOff() {
        isOn = false;
    }

    // Get device status
    bool getStatus() const {
        return isOn;
    }

    // Display device information
    void showInfo() const {
        cout << "Name: " << name
            << ", Status: " << (isOn ? "ON" : "OFF")
            << ", Brightness: " << brightness << endl;
    }
};

// Room class manages multiple devices
class Room {
private:
    string roomName;     // room name
    Device devices[5];   // fixed-size device array
    int count;           // current number of devices

public:
    // Constructor
    Room(string name) {
        roomName = name;
        count = 0;
    }

    // Destructor
    ~Room() {
        cout << "Room [" << roomName << "] manager closed.\n";
    }

    // Add new device (max 5)
    void addDevice(Device d) {
        if (count < 5) {
            devices[count] = d; // store device
            count++;
            cout << "Device added successfully.\n";
        }
        else {
            cout << "Room is full.\n";
        }
    }

    // Show all devices
    void showAllDevices() const {
        if (count == 0) {
            cout << "No devices in room.\n";
            return;
        }

        for (int i = 0; i < count; i++) {
            devices[i].showInfo();
        }
    }

    // Find device index by name
    int findDevice(string name) {
        for (int i = 0; i < count; i++) {
            if (devices[i].getName() == name)
                return i;
        }
        return -1; // not found
    }

    // Toggle ON/OFF state
    void toggleDevice(string name) {
        int index = findDevice(name);

        if (index == -1) {
            cout << "Device not found.\n";
            return;
        }

        // Switch state
        if (devices[index].getStatus())
            devices[index].turnOff();
        else
            devices[index].turnOn();

        cout << "Device toggled successfully.\n";
    }

    // Update brightness
    void setDeviceBrightness(string name, int b) {
        int index = findDevice(name);

        if (index == -1) {
            cout << "Device not found.\n";
            return;
        }

        devices[index].setBrightness(b);
        cout << "Brightness updated successfully.\n";
    }

    // Save devices to file
    void saveToFile() {
        ofstream file("devices.txt");

        file << "Total Devices: " << count << endl << endl;

        for (int i = 0; i < count; i++) {
            file << "Name: " << devices[i].getName() << endl;
            file << "Status: " << (devices[i].getStatus() ? "ON" : "OFF") << endl;
            file << "Brightness: " << devices[i].getBrightness() << endl;
            file << "--------------------" << endl;
        }

        file.close(); // close file
    }

    // Load devices from file
    void loadFromFile() {
        ifstream file("devices.txt");

        if (!file)
            return; // file not found

        file >> count;    // read device count
        file.ignore();    // clear newline

        for (int i = 0; i < count; i++) {
            string name;
            bool status;
            int brightness;

            getline(file, name);
            file >> status;
            file >> brightness;
            file.ignore();

            devices[i].setName(name);

            if (status)
                devices[i].turnOn();
            else
                devices[i].turnOff();

            devices[i].setBrightness(brightness);
        }

        file.close();
    }
};

int main() {
    Room room("My Room"); // create room object

    room.loadFromFile();  // load saved data

    int choice;

    do {
        cout << "\n=== Smart Room Manager ===\n";
        cout << "1. Add Device\n";
        cout << "2. Show All Devices\n";
        cout << "3. Turn Device ON/OFF\n";
        cout << "4. Change Brightness\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            string name;
            cout << "Enter device name: ";
            cin >> name;

            Device d;       // create device
            d.setName(name);

            room.addDevice(d);
        }
        else if (choice == 2) {
            room.showAllDevices();
        }
        else if (choice == 3) {
            string name;
            cout << "Enter device name: ";
            cin >> name;

            room.toggleDevice(name);
        }
        else if (choice == 4) {
            string name;
            int b;

            while (true) {
                cout << "Enter device name: ";
                cin >> name;

                int index = room.findDevice(name);

                if (index == -1) {
                    cout << "Device not found. Try again.\n";
                }
                else {
                    cout << "Enter brightness (0-100): ";
                    cin >> b;

                    room.setDeviceBrightness(name, b);
                    break;
                }
            }
        }

    } while (choice != 5);

    room.saveToFile(); // save before exit

    return 0;
}