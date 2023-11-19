#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <cmath>
#include <queue>

using namespace std;

// Global data structures:

// Maps hashed usernames to hashed passwords
map<long long, long long> hashtable; 
// Maps hashed usernames to actual usernames
map<long long, string> names; 
// Vector below Represents the friendship graph
vector<vector<long long>> graph(2e5 + 1, vector<long long> ()); 
// Levels used for BFS to find shortest path between two users
vector<long long> levels(2e5 + 1); 
// visited vector marks whether a user has been visited during BFS
vector<bool> visited(2e5 + 1); 
// Stores status messages for each user in string formats
vector<string> status(2e5 + 1); 

// Function to hash a username
long long UsernameHash(string username)
{
    long long ModValue = 10139;
    long long EncryptedUsername = 0;

    for(int x = 0 ; x < username.size() ; x++)
    {
        long long factor = (abs(username[x] - 'a' + 1)) * pow((x+1), x);
        factor %= ModValue;
        EncryptedUsername += factor;
        EncryptedUsername %= ModValue;
    }

    return EncryptedUsername;
}

// Function to hash a password
long long PasswordHash(string Password)
{
    long long ModValue = 10139;
    long long EncryptedPassword = 0;

    for(int x = 0 ; x < Password.size() ; x++)
    {
        long long factor = (abs(Password[x] - 'a' + 1)) * pow((x+1), x);
        factor %= ModValue;
        EncryptedPassword += factor;
        EncryptedPassword %= ModValue;
    }

    return EncryptedPassword;
}

// Function to authenticate a user
bool authenticate(string username, string password)
{
    long long user = UsernameHash(username);
    long long passwd = PasswordHash(password);

    if(hashtable[user] == passwd)
        return true;
    else
        return false;
}

// Function to check if a username already exists
bool Existence(long long username)
{
    map<long long,long long> :: iterator exists;
    exists = hashtable.find(username);

    if(exists == hashtable.end())
        return false;
    else
        return true;
}

// Function to add a status message
string AddStatus()
{
    cout << "Enter No. of Words In Your Status : ";
    long long words;
    cin >> words;
    string stat = "";
    string dummy;

    while(words--)
    {
        cin >> dummy;
        stat += dummy;
        stat += ' ';
    }

    stat += '.';

    return stat;
}

// Function to add a new user
void addUser()
{
    string username;
    string password;

    cout << "\nEnter A Username : ";
    cin >> username;

    long long EncryptedUsername = UsernameHash(username);

    if(Existence(EncryptedUsername))
        cout << "Error! Username has already been taken by someone.\n\n";
    else
    {
        cout << endl << "Enter A Password : ";
        cin >> password;
        long long EncryptedPassword = PasswordHash(password);
        hashtable[EncryptedUsername] = EncryptedPassword;
        names[EncryptedUsername] = username;
        cout << "\nUser has been Added!\n";
    }

    cout << endl << "Enter the Message You'd Like to Display as your Status: -" << endl << endl;

    status[EncryptedUsername] = AddStatus();

    cout << endl << "Your Status Has Been Successfully Set! ";

    return;
}

// Function to delete a user
void DeleteUser()
{   
    string username, password;

    cout << "Enter Your Username : ";
    cin >> username;

    long long EncryptedUsername = UsernameHash(username);

    if(!Existence(EncryptedUsername))
        cout << "Error! Incorrect Username Entered.\n\n";
    else
    {
        cout << "Enter Your Password : ";
        cin >> password;
        long long EncryptedPassword = PasswordHash(password);

        if(hashtable[EncryptedUsername] == EncryptedPassword)
        {
            map<long long, long long>::iterator found = hashtable.find(EncryptedUsername);
            hashtable.erase(found);

            cout << "\nUser has been Deleted!\n";

            for(long long x = 0 ; x < graph.size() ; x++)
            {
                vector<long long> :: iterator found = find(graph[x].begin(), graph[x].end(), EncryptedUsername);

                if(found != graph[x].end())
                    graph[x].erase(found);
            }

            cout << endl << "The User Has Also Been Removed From The Friend-Lists of All Active Users." << endl;
        }
    }
    
    return;
}

// Function to add friends
void AddFriends(string username1, string username2)
{
    long long user1 = UsernameHash(username1);
    long long user2 = UsernameHash(username2);

    if(Existence(user1) && Existence(user2))
    {
        graph[user1].push_back(user2);
        cout << "The Two Users Have Been Successfully Made Friends! \n\n";
        return;
    }
    else
    {
        cout << "Invalid Usernames Entered! \n\n";
        return;
    }
}

// Function to remove friends
void RemoveFriends(string username1, string username2)
{
    long long user1 = UsernameHash(username1);
    long long user2 = UsernameHash(username2);

    if(!Existence(user1) || !Existence(user2))
    {
        cout << "Invalid Usernames Entered! " << endl << endl;
        return;
    }
    else
    {
        bool alreadyfriends = false;

        for(long long x = 0 ; x < graph[user1].size() ; x++)
            if(graph[user1][x] == user2)
            {
                alreadyfriends = true;
                break;
            }

        if(!alreadyfriends)
        {
            cout << "Users Aren't Friends Already! " << endl << endl;
            return;
        }
        else
        {
            vector<long long> :: iterator it;

            for(int x = 0 ; x < graph[user1].size() ; x++)
                if(graph[user1][x] == user2)
                {
                    it = graph[user1].begin() + x;
                    break;
                }

            graph[user1].erase(it);

            cout << "The Procedure has Been Completed! " << endl << endl;
            cout << username2 << " Has Been Removed From " << username1 << "'s Friend List... " << endl << endl;
        }
    }
}

// Function to find mutual friends
void Mutuals(string user1, string user2)
{
    long long root = UsernameHash(user1);

    queue <long long> q;

    q.push(root);

    visited[root] = true;

    while(!q.empty())
    {
        long long current = q.front();
        q.pop();
        visited[current] = true;

        for(auto neighbour : graph[current])
            if(!visited[neighbour])
            {
                levels[neighbour] = 1 + levels[current];
                q.push(neighbour);
                visited[neighbour] = true;
            }
    }

    long long user2hash = UsernameHash(user2);

    cout << endl << "Shortest Count of Users Between " << user1 << " & " << user2 << " is : " << levels[user2hash] << endl;
}

// Function to exit the program
void Exit()
{
    cout << endl << "Terminating the Program... " << endl << endl;
    cout << "Thanks For Using WeChat! " << endl << endl;
    return;
}

// Main function
int main()
{
    string input = "proceed";

    while(input != "KillTheProgram#404")
    {
        cout << endl << endl << "Welcome to Our Program!" << endl << endl;

        cout << "What Would You Like To Do? (Please Choose An Option From the List) :- " << endl << endl;
        cout << "1. Log-In (LI). " << endl;
        cout << "2. Sign-Up (SU). " << endl;
        cout << "Exit (Type Special Command)" << endl << endl;

        cin >> input;

        if(input == "LI")
        {
            string name, password;
            cout << endl << "Enter Your Username : ";
            cin >> name;
            long long hashedUsername = UsernameHash(name);
            bool found = Existence(hashedUsername);

            if(!found)
            {
                cout << endl << "Error. Invalid Username Entered! " << endl << endl;
                cout << endl << "Returning to Main Menu..." << endl << endl;
                continue;
            }

            cout << endl << "Enter Your Password : ";
            cin >> password;

            long long hashedPassword = PasswordHash(password);

            bool auth = authenticate(name, password);

            if(!auth)
            {
                cout << endl << "Error. Invalid Password Entered! " << endl << endl;
                cout << endl << "Returning to Main Menu..." << endl << endl;
                continue;
            }

            cout << endl << "Congratulations " << name << "! You Have Been Successfully Logged-In.." << endl << endl;

            while(input != "GB")
            {
                cout << "What Would You Like to Do? " << endl << endl;
                cout << "1. View Current Friends (VCF)." << endl << endl;
                cout << "2. Add A Friend (AF)." << endl << endl;
                cout << "3. Remove A Friend (RF)." << endl << endl;
                cout << "4. View Mutuals (VM)." << endl << endl;
                cout << "5. Delete Your Account (DA)." << endl << endl;
                cout << "6. Your Profile Details (PD). " << endl << endl;
                cout << "7. Edit Status (ES). " << endl << endl;
                cout << "8. Edit Password (EP). " << endl << endl;
                cout << "9. View Friend's Status (VFS). " << endl << endl;
                cout << "10. Go Back (GB). " << endl << endl;
                cin >> input;

                if(input == "VCF")
                {
                    cout << endl << "Displaying Current Friend List: - " << endl << endl;
                    for(long long x = 0 ; x < graph[hashedUsername].size() ; x++)
                        cout << "#" << x + 1 << " : " << names[graph[hashedUsername][x]] << endl << endl;

                    cout << "In Total, You Have " << graph[hashedUsername].size() << " Friends." << endl << endl;
                    system("pause");
                }
                else if(input == "AF")
                {
                    string friendsName;
                    cout << endl << "Enter Your Friend's Name : " << endl << endl;
                    cin >> friendsName;
                    long long hashedfriendsName = UsernameHash(friendsName);
                    bool found = Existence(hashedfriendsName);

                    if(!found)
                    {
                        cout << "Error. Invalid Username Entered! " << endl << endl;
                        cout << "Returning to Main Menu..." << endl << endl;  
                        continue;
                    }

                    else
                        AddFriends(name,friendsName);
                    system("pause");
                }

                else if(input == "RF")
                {
                    string friendsName;
                    cout << endl << "Enter Your Friend's Name : " << endl << endl;
                    cin >> friendsName;
                    long long hashedfriendsName = UsernameHash(friendsName);
                    bool found = Existence(hashedfriendsName);

                    if(!found)
                    {
                        cout << "Error. Invalid Username Entered! " << endl << endl;
                        cout << "Returning to Main Menu..." << endl << endl;
                        continue;
                    }

                    else 
                        RemoveFriends(name,friendsName);
                    system("pause");
                }

                else if(input == "VM")
                {
                    string friendsName;
                    cout << endl << "Enter Your Friend's Name : " << endl << endl;
                    cin >> friendsName;
                    long long hashedfriendsName = UsernameHash(friendsName);
                    bool found = Existence(hashedfriendsName);

                    if(!found)
                    {
                        cout << "Error. Invalid Username Entered! " << endl << endl;
                        cout << "Returning to Main Menu..." << endl << endl;
                        continue;
                    }

                    else
                        Mutuals(name,friendsName);
                    system("pause");
                }

                else if(input == "DA")
                {
                    DeleteUser();
                    break;
                }
                
                else if(input == "PD")
                {
                    cout << endl << "Displaying User Details : - \n\n";
                    cout << "Name : " << names[hashedUsername] << endl << endl;
                    cout << "Password : ";

                    for(long long x = 0 ; x < password.size()/2 ; x++)
                        cout << " * ";
                    
                    for(long long x = password.size() / 2 ; x < password.size() ; x++)
                        cout << ' ' << password[x] << ' ';
                    
                    cout << endl << endl;
                    cout << "Status : " << status[hashedUsername] << endl << endl;
                    system("pause");
                }

                else if(input == "EP")
                {
                    cout << endl << "Enter New Password : ";
                    string pass;
                    cin >> pass;
                    long long hashedpass = PasswordHash(pass);
                    hashtable[hashedUsername] = hashedpass;
                    cout << endl << "Your Password Has Been Successfully Changed! " << endl;
                    system("pause");
                }

                else if(input == "ES")
                {
                    cout << endl << "Initiating Status Updation Procedure... " << endl << endl;
                    string newstatus = AddStatus();
                    status[hashedUsername] = newstatus;
                    cout << endl << "Your Status Has Been Updated!" << endl << endl;
                    system("pause");
                }

                else if(input == "VFS")
                {
                    string friendsName;
                    cout << endl << "Enter Your Friend's Name : " << endl << endl;
                    cin >> friendsName;
                    long long hashedfriendsName = UsernameHash(friendsName);
                    bool found = Existence(hashedfriendsName);

                    if(!found)
                    {
                        cout << "Error. Invalid Username Entered! " << endl << endl;
                        cout << "Returning to Main Menu..." << endl << endl;
                        continue;
                    }

                    vector<long long> :: iterator it = find(graph[hashedUsername].begin(), graph[hashedUsername].end(), hashedfriendsName);
                    if(it == graph[hashedUsername].end())
                    {
                        cout << "Error. This Person Is Not Your Friend! " << endl << endl;
                        cout << "Returning to Main Menu..." << endl << endl;
                        continue;
                    }

                    cout << endl << friendsName << "'s Status is : - \n\n";
                    cout << status[hashedfriendsName] << endl << endl;
                    system("pause");
                }
                else if(input == "GB")
                    break;
            }
        }

        else if(input == "SU")
        {
            cout << endl << "Initiating Registration Procedure.." << endl;
            addUser();
            system("pause");
        }
    }
    Exit();
    return 0;
}
