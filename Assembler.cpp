#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <bitset>

using namespace std;

#define endl "\n"
#define fast_io                        \
    ios_base::sync_with_stdio(false);  \
    cin.tie(0);                        \
    cout.tie(0);

struct Instruction {
    string name;
    string hexCode;
    string binaryCode;
};

string hexToBinary(const string& hex) {
    map<char, string> hexToBin = {
        {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"},
        {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"},
        {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"},
        {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"}
    };

    string binary = "";

    for (char c : hex) {
        c = toupper(c);
        if (hexToBin.find(c) != hexToBin.end()) {
            binary += hexToBin[c];
        }
        else {
            cerr << "Invalid hex character: " << c << endl;
            return "";
        }
    }

    while (binary.length() < 16) {
        binary = "0" + binary;
    }
    return binary;
}

string hexToBinary1(const string& hex) {
    map<char, string> hexToBin = {
        {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"},
        {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"},
        {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"},
        {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"}
    };

    string binary = "";

    for (char c : hex) {
        c = toupper(c);
        if (hexToBin.find(c) != hexToBin.end()) {
            binary += hexToBin[c];
        }
        else {
            cerr << "Invalid hex character: " << c << endl;
            return "";
        }
    }

    while (binary.length() < 12) {
        binary = "0" + binary;
    }
    return binary;
}

string decToBinary(int decimal) {
    if (decimal >= 0) {
        return bitset<16>(decimal).to_string();
    }
    else {
        return bitset<16>(65536 + decimal).to_string();
    }
}

string DTohex(int d) {
    stringstream ss;
    ss << hex << uppercase << d; // Convert to hexadecimal and make uppercase
    return ss.str();
}

void solve() {
    vector<Instruction> instructions = {
        {"AND", "0 or 8", "0000 or 1000"},
        {"ADD", "1 or 9", "0001 or 1001"},
        {"LDA", "2 or A", "0010 or 1010"},
        {"STA", "3 or B", "0011 or 1011"},
        {"BUN", "4 or C", "0100 or 1100"},
        {"BSA", "5 or D", "0101 or 1101"},
        {"ISZ", "6 or E", "0110 or 1110"},
        {"CLA", "7800", "0111100000000000"},
        {"CLE", "7400", "0111010000000000"},
        {"CMA", "7200", "0111001000000000"},
        {"CME", "7100", "0111000100000000"},
        {"CIR", "7080", "0111000010000000"},
        {"CIL", "7040", "0111000001000000"},
        {"INC", "7020", "0111000000100000"},
        {"SPA", "7010", "0111000000010000"},
        {"SNA", "7008", "0111000000001000"},
        {"SZA", "7004", "0111000000000100"},
        {"SZE", "7002", "0111000000000010"},
        {"HLT", "7001", "0111000000000001"},
        {"INP", "F800", "1111100000000000"},
        {"OUT", "F400", "1111010000000000"},
        {"SKI", "F200", "1111001000000000"},
        {"SKO", "F100", "1111000100000000"},
        {"ION", "F080", "1111000010000000"},
        {"IOF", "F040", "1111000001000000"}
    };

    vector<string> ar;
    vector<string> memory; // Memory to store machine language output

    string line;
    while (getline(cin, line)) {
        if (line.find("END") != string::npos) break;
        ar.push_back(line);
    }

    map<string, int> labelTable;
    int initial = (ar[0].find("ORG") != string::npos ) ? stoi(ar[0].substr(ar[0].find("ORG") + 4), 0, 16) - 1 : 0 ;

    for (int i = 0; i < ar.size(); i++) {
        string line = ar[i];
        if (line.find(',') != string::npos) {
            string label = line.substr(0, line.find(','));
            labelTable[label] = initial + i;
        }
    }

    cout << "Table of Labels" << endl;
    for (const auto& entry : labelTable) {
        cout << hex << uppercase <<  entry.first << " : " << entry.second << endl;
    }

    cout << "Machine Language : " << endl;
    for (int i = 0; i < ar.size(); i++) {
        if (ar[i].find("END") != string::npos) {
            break;
        }
        else {
            int r = 0;
            for (const auto& instr : instructions) {
                r++;
                if (ar[i].find(instr.name) != string::npos && r > 6) {
                    string output = ": " + instr.binaryCode;
                    cout << hex << uppercase << initial + i;
                    memory.push_back(output);
                    cout << output << endl;
                    break;
                }
                else if (ar[i].find(instr.name) != string::npos) {
                    string opcode = ar[i].substr(4, 3);
                    string binaryadd = "";
                    if (ar[i].find(instr.name) != string::npos) {
                        string binaryOutput;

                        if (ar[i].find("I") != string::npos) {
                            binaryOutput = instr.binaryCode.substr(8);
                        }
                        else {
                            binaryOutput = instr.binaryCode.substr(0, 4);
                        }

                            if (labelTable.find(opcode) != labelTable.end()) {
                                int labelAddress = labelTable[opcode];
                                binaryadd = hexToBinary1(DTohex(labelAddress));
                            }
                            string output = ": " + binaryOutput + binaryadd;
                            cout << hex << uppercase << initial + i;
                            memory.push_back(output);
                            cout << output << endl;


                    }
                    break;
                }
                else if (ar[i].find("DEC") != string::npos) {
                    string binarycode = decToBinary(stoi(ar[i].substr(ar[i].find("DEC") + 4)));
                    string output = ": " + binarycode;
                    cout << hex << uppercase << initial + i;
                    memory.push_back(output);
                    cout << output << endl;
                    break;
                }
                else if (ar[i].find("HEX") != string::npos) {
                    string binarycode = hexToBinary(ar[i].substr(ar[i].find("HEX") + 4));
                    string output = ": " + binarycode;
                    cout << hex  << uppercase << initial + i;
                    memory.push_back(output);
                    cout << output << endl;
                    break;
                }
            }
        }
    }
}

int main() {
    fast_io;
    solve();
    return 0;
}
