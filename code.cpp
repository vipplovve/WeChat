#include <bits/stdc++.h>
using namespace std;

map<long long, long long> hashtable;

long long UsernameHash(string username)
{
    long long ModValue = 98746231;

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
    long long ModValue = 10235749;

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

int main()
{
    return 0;
}