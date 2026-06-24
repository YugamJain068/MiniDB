#include <iostream>
#include <string>

using namespace std;

int main() {

    string query;

    while (true) {

        cout << "MiniDB > ";

        getline(cin, query);

        if (query == "EXIT") {
            break;
        }

        cout << "You entered: " << query << endl;
    }

    return 0;
}