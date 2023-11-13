#include <bits/stdc++.h>

using namespace std;

map<long long, long long> hashtable;

vector<vector<long long>> graph(2e5 + 1, vector<long long> ());

vector<long long> levels(2e5 + 1);

vector<bool> visited(2e5 + 1);

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

bool authenticate(string username, string password)
{
    long long user = UsernameHash(username);
    long long passwd = PasswordHash(password);

    if(hashtable[user] == passwd)
        return true;
    
    else
        return false;
}

bool Existence(long long username)
{
    map<long long,long long>::iterator exists;

    exists = hashtable.find(username);

    if(exists != hashtable.end())
        return false;

    else
        return true;
}

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
        cout << "Enter A Password : ";

        cin >> password;

        long long EncryptedPassword = PasswordHash(password);

        hashtable[EncryptedUsername] = EncryptedPassword;

        cout << "\nUser has been Added!\n";
    }

    return;
}

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
        }
    }
    
    return;
}

void AddFriends(string username1, string username2)
{
    long long user1 = UsernameHash(username1);

    long long user2 = UsernameHash(username2);

    if(Existence(user1) && Existence(user2))
    {
        graph[user1].push_back(user2);
        graph[user2].push_back(user1);

        cout << "The Two Users Have Been Successfully Made Friends! \n\n";

        return;
    }

    else
    {
        cout << "Invalid Usernames Entered! \n\n";

        return;
    }
}

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

int main()
{

    return 0;
}