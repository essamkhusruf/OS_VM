#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <openssl/des.h>

using namespace std;

// encrypt password
string encryptPassword(string password, unsigned short salt) {
    DES_cblock key;
    DES_key_schedule schedule;

    // make key from password (max 8 chars)
    for (int i = 0; i < 8; i++) {
        if (i < password.size())
            key[i] = password[i];
        else
            key[i] = 0;
    }

    DES_set_key_unchecked(&key, &schedule);

    DES_cblock input = {0};
    DES_cblock output;

    // put salt in input
    input[0] = salt & 0xFF;
    input[1] = (salt >> 8) & 0xFF;

    // 25 times encryption
    for (int i = 0; i < 25; i++) {
        DES_ecb_encrypt(&input, &output, &schedule, DES_ENCRYPT);
        for (int j = 0; j < 8; j++) input[j] = output[j];
    }

    return string((char*)output, 8);
}

// check password
bool checkPassword(string input, string stored, unsigned short salt) {
    return encryptPassword(input, salt) == stored;
}

// print hex
void printHex(string s) {
    for (int i = 0; i < s.size(); i++) {
        printf("%02X", (unsigned char)s[i]);
    }
    cout << endl;
}

int main() {
    srand(time(0));

    vector<string> passwords = {
        "pass1", "hello", "admin", "test",
        "abc", "os", "linux", "user",
        "aaa", "bbb"
    };

    vector<string> hashes;
    vector<unsigned short> salts;

    // generate 10 encrypted passwords
    for (int i = 0; i < 10; i++) {
        unsigned short salt = rand() % 65536;

        string h = encryptPassword(passwords[i], salt);

        salts.push_back(salt);
        hashes.push_back(h);

        cout << "Password: " << passwords[i] << endl;
        cout << "Salt: " << salt << endl;
        cout << "Hash: ";
        printHex(h);
        cout << "-------------------\n";
    }

    // test check
    string input;
    cout << "Enter password to check first one: ";
    cin >> input;

    if (checkPassword(input, hashes[0], salts[0]))
        cout << "Correct\n";
    else
        cout << "Wrong\n";

    return 0;
}
