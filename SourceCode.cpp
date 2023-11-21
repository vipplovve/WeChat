#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <cmath>
#include <queue>

using namespace std;

// Global data structures ૮˶ᵔᵕᵔ˶ა :

// Maps hashed usernames to hashed passwords
map<long long, long long> hashtable;

// Maps hashed usernames to actual usernames
map<long long, string> names;

// Vector below Represents the friendship graph
vector<vector<long long>> graph(2e5 + 1, vector<long long> ());

// Levels used for BFS to find shortest path between two users
vector<long long> levels(2e5 + 1, 0);

// visited vector marks whether a user has been visited during BFS
vector<bool> visited(2e5 + 1, false);

// Stores status messages for each user in string formats
vector<string> status(2e5 + 1);


/* Function to hash a username:this function is
taking username in string format entered by the user 
and converting it into EncryptedUsername by using below formulations*/
long long UsernameHash(string username)
{
    //Initialised ModValue as 10139 which specifies the maximum range
    long long ModValue = 10139;
    
    //EncryptedUsername initialised with 0 which gets 
    //updated for each username being hashed
    long long EncryptedUsername = 0;
    
    //Iterates through each character of the input username using a for loop.
    for(int x = 0 ; x < username.size() ; x++)
    {
        //abs(username[x]-'a'+1) calculates the position of the character in the alphabets string.(1-indexed) 
        //(x+1) is the position of the character in the string (1-indexed).
        //pow((x+1), x) raises the position to the power of itself.
        
        //yeah, we know formula is bit complicated (ס_ס;;)
        long long factor = (abs(username[x] - 'a' + 1)) * pow((x+1), x);
        
        //Calculated factor modulo ModValue if in case factor exceeds the range
        factor %= ModValue;

        EncryptedUsername += factor;
        
        //Takes EncryptedUsername modulo ModValue to ensure the result stays within the range of ModValue.
        EncryptedUsername %= ModValue;
    }

    return EncryptedUsername;
}


/* Function to hash a password: this function is
taking Password in string format entered by the user 
and converting it into EncryptedPassword by using below formulations*/
long long PasswordHash(string Password)
{
    //Initialised ModValue as 10139 which specifies the maximum range
    long long ModValue = 10139;

    //EncryptedPassword initialised with 0 which gets 
    //updated for each password being hashed
    long long EncryptedPassword = 0;

    //Iterates through each character of the input Password using a for loop.
    for(int x = 0 ; x < Password.size() ; x++)
    {
        //abs(Password[x]-'a'+1) calculates the position of the character in the alphabets string.(1-indexed) 
        //(x+1) is the position of the character in the string (1-indexed).
        //pow((x+1), x) raises the position to the power of itself.
        
        //yeah, we know formula is bit complicated (ס_ס;;)
        long long factor = (abs(Password[x] - 'a' + 1)) * pow((x+1), x);

        //Calculated factor modulo ModValue if in case factor exceeds the range 
        factor %= ModValue;
        
        EncryptedPassword += factor;
        
        //Takes EncryptedPassword modulo ModValue to ensure the result stays within the range of ModValue.
        EncryptedPassword %= ModValue;
    }

    return EncryptedPassword;
}


/* this function is taking a username and password in input
in form of strings and authenticating whether the given 
password corresponds correctly to the given user or not */  
bool authenticate(string username, string password)
{
    long long user = UsernameHash(username);

    long long passwd = PasswordHash(password);
    
    //if passwd(value) corresponds to respective user(key)
    if(hashtable[user] == passwd)
        //then return true for further operations
        return true;
    
    else
        //return false if any password dont match with its user
        return false;
}


/* This function is taking a username as an input
and verifying whether a user with this username exists 
or not */
bool Existence(long long username)
{
    //iterator named exists to traverse the map 
    map<long long,long long> :: iterator exists;
    
    //.find(variable) is stl in <map> library which returns
    //index of first occurrence of the variable 'username'
    exists = hashtable.find(username);

    //.end() returns the index after the last index of the map
    //if exists equal to hashtable.end() it means username not found
    if(exists == hashtable.end())
        return false;

    else
        return true;
}


/* By using this function a user can 
add a status to their profile to 
give their update to their friends */
string AddStatus()
{ 
    //the status will be in text format only
    //Sorry for no images or videos :(
    cout << "Enter No. of Words In Your Status : ";

    long long words;
    
    //update the space according to the number of words entered by user
    cin >> words;

    string stat = "";
    
    string dummy;

    while(words--)
    {
        cin >> dummy;
        
        //concatenating a word 'dummy' to string 'stat'
        stat += dummy;
        
        //space after each word
        stat += ' ';
    }
    
    //Since, Democracy decided to have a fullstop at the 
    //end of each sentence, so here's a fullstop for your status ♥(ˆ⌣ˆԅ)
    stat += '.';

    return stat;
}


/* this function is to create a new profile 
on Wechat for a user and will ask for status upon 
successfull registration*/
void addUser()
{
    string username;

    string password;

    cout << "\nEnter A Username : ";

    cin >> username;
    
    //will call the function UsernameHash() with username as input
    long long EncryptedUsername = UsernameHash(username);
    
    //check whether username entered is already present or not
    if(Existence(EncryptedUsername))
        cout << "Error! Username has already been taken by someone.\n\n";
    
    //if not present then only our program will ask for password
    else
    {
        cout << endl << "Enter A Password : ";

        cin >> password;
        
        //will call the function PasswordHash() with password as input 
        long long EncryptedPassword = PasswordHash(password);
        
        //make a key value pair of Username with its Password and add into hashtable
        hashtable[EncryptedUsername] = EncryptedPassword;
        
        //add username into names map
        names[EncryptedUsername] = username;

        cout << "\nUser has been Added!\n";
    }

    cout << endl << "Enter the Message You'd Like to Display as your Status: -" << endl << endl;
    
    //will call AddStatus() function 
    status[EncryptedUsername] = AddStatus();

    cout << endl << "Your Status Has Been Successfully Set! ";

    return;
}


/* this function is for deleting a
user profile from WeChat */
void DeleteUser()
{   
    string username, password;

    cout << "Enter Your Username : ";
    
    //ask for username to be deleted
    cin >> username;
    
    //will call UsernameHash() function
    long long EncryptedUsername = UsernameHash(username);
    
    //check if username present or not
    if(!Existence(EncryptedUsername))
        cout << "Error! Incorrect Username Entered.\n\n";
        
    //if present then only run delete operations
    else
    {
        cout << "Enter Your Password : ";
        
        //ask password for authentication
        cin >> password;

        long long EncryptedPassword = PasswordHash(password);
        
        //if password correspond to username then go with the process otherwise not
        if(hashtable[EncryptedUsername] == EncryptedPassword)
        {   
            //iterator to find the username
            map<long long, long long>::iterator found = hashtable.find(EncryptedUsername);
            
            //.erase(iterator) is stl in map library to delete the value corresponding to iterator
            hashtable.erase(found);

            cout << "\nUser has been Deleted!\n";
            
            for(long long x = 0 ; x < graph.size() ; x++)
            {   
                //graph iterator to find the username node in our connected graph
                vector<long long> :: iterator found = find(graph[x].begin(), graph[x].end(), EncryptedUsername);
                
                //if found, will delete the username from our graph along with its connections
                if(found != graph[x].end())
                    graph[x].erase(found);
            }

            cout << endl << "The User Has Also Been Removed From The Friend-Lists of All Active Users." << endl;
            cout << "\nThanks for being our family member, we will miss you. (◞‸◟；) ♡\n";
        }
    }
    
    return;
}

/* this function is for making two different users,
friend of each other ( technically linking their profiles )
on wechat so that they can use multiple features provided on
we chat for friends */
void AddFriends(string username1, string username2)
{
    //will call UsernameHash() function and convert it into Encrypted format
    long long user1 = UsernameHash(username1);
    long long user2 = UsernameHash(username2);

    //call Existence() function to check whether users are registered users or not
    if(Existence(user1) && Existence(user2))
    {
        //will add user2 in the friend list of user1 and vice versa
        //means a connection(edge) is created between the two users(vertices)
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


/* this function is to unfriend 
any two profiles on wechat */
void RemoveFriends(string username1, string username2)
{
    //will call UsernameHash() function and convert it into Encrypted format
    long long user1 = UsernameHash(username1);
    long long user2 = UsernameHash(username2);

    //call Existence() function to check whether any of users is registered or not
    if(!Existence(user1) || !Existence(user2))
    {
        cout << "Invalid Usernames Entered! " << endl << endl;

        return;
    }

    else
    {
        bool alreadyfriends = false;
        
        //traverse the friend list of user1 
        for(long long x = 0 ; x < graph[user1].size() ; x++)
            //if user2 is found as friend of user1 in graph
            if(graph[user1][x] == user2)
            {   
                //change the bool value to 1
                alreadyfriends = true;
                break;
            }
        
        if(!alreadyfriends)
        {
            cout << "Users Aren't Friends Already! " << endl << endl;

            return;
        }

        //if alreadyfriends is true then only delete
        else
        {
            //vector iterator to traverse 
            vector<long long> :: iterator it;

            //traversing the graph until found the user2 as friend of user1
            for(int x = 0 ; x < graph[user1].size() ; x++)
                if(graph[user1][x] == user2)
                {
                    //converting iterator to indexing
                    it = graph[user1].begin() + x;
                    break;
                }
                
            //simply erase the value at it index which is user2 only
            //it will break the connection between user1 and user2
            graph[user1].erase(it);

            cout << "The Procedure has Been Completed! " << endl << endl;

            cout << username2 << " Has Been Removed From " << username1 << "'s Friend List... " << endl << endl;
        }
    }
}


/* this function makes 2 friends mutuals if 
they have a common friend by calculating shortest path or count of users
between two users*/
void DistinctUserCount(string user1, string user2)
{
    //will call UsernameHash() function and convert it into Encrypted format
    long long root = UsernameHash(user1);
    
    //Initializes a queue to perform a breadth-first search (BFS) traversal of the graph or network. 
    queue <long long> q;

    q.push(root);
    
    
    visited[root] = true;
    
    //Performs a BFS traversal by continuously popping nodes from the queue and marking them as visited.
    while(!q.empty())
    {
        //pick the front node from queue into current
        long long current = q.front();
        //pop or remove the node current from queue as no need in future
        q.pop();

        visited[current] = true;
        
        /*For each node 'current' from the queue, it iterates through its neighbors 
        in graph and updates their levels or distance if they haven't been visited yet. 
        It also pushes these unvisited neighbors into the queue.*/
        for(auto neighbour : graph[current])
            if(!visited[neighbour])
            {
                //increment levels each time neighbour found
                levels[neighbour] = 1 + levels[current];
                
                //pushed the neighbours into queue
                q.push(neighbour);
                
                visited[neighbour] = true;
            }
    }
    
    long long user2hash = UsernameHash(user2);
    
    //check if user2 is present in levels 
    //if present then print the shortest count of users 
    if(levels[user2hash])
        cout << endl << "Shortest Count of Users Between " << user1 << " & " << user2 << " is : " << levels[user2hash] - 1 << endl;
    
    //otherwise no connection between user1 and user2
    else 
        cout << endl << "The Two Users Have No Common Friends." << endl << endl;

    levels.assign(2e5 + 1, 0);
    visited.assign(2e5 + 1, false);
}

/*this function terminates out of the 
terminal and ends the program */
void Exit()
{
    cout << endl << "Terminating the Program... " << endl << endl;

    cout << "Thanks For Using WeChat! (づ๑•ᴗ•๑)づ♡" << endl << endl;

    return;
}

/* this is an additional feature in wechat ,
whenever a user calls this function a random greeting is 
generated which will be different from previous upto 9 times 
in best case */
void random_greeting()
{
    int randomNum = (rand()*15815) % 10;

    cout << endl;

    switch(randomNum)
        {
            case 1:
            cout<<"The silence in the morning holds lots of expectations and is more hopeful than the silence at night."<<endl;
            break;
            case 2:
            cout<<"I like my coffee black and my mornings bright."<<endl;
            break;
            case 3:
            cout<<" The next morning dawned bright and sweet, like ribbon candy "<<endl;
            break;
            case 4:
            cout<< " You can only come to the morning through the shadows. " <<endl;
            break;
            case 5:
            cout<<  " Every morning is a beautiful morning.  " <<endl;
            break;
            case 6:
            cout<< "  Make each day your masterpiece  "  <<endl;
            break;
            case 7:
            cout<< "  Not the day only, but all things have their morning  "  <<endl;
            break;
            case 8:
            cout<< "  The sun is new each day.  "  <<endl;
            break;
            case 9:
            cout<< "  Prayer is the key of the morning and the bolt of the evening.  "  <<endl;
            break;
        }

}


// here is our main function //

int main()
{
    string input = "proceed";

    while(input != "KillTheProgram#404")
    {
        cout << endl << endl << "Welcome to Our Program! o(◑ω◐)づ"<< endl << endl;

        cout << "What Would You Like To Do? (Please Choose An Option From the List) :- " << endl << endl;

        //you need to either sign in or sign up 
        
        //Enter LI into terminal for Login In
        cout << "1. Log-In (LI). " << endl;
        
        //Enter SU into terminal for Sign Up
        cout << "2. Sign-Up (SU). " << endl;

        cout << "Exit (Type Special Command)" << endl << endl;

        cin >> input;
        
        //You can login only if you have an account associate with our program
        if(input == "LI")
        {
            string name, password;

            cout << endl << "Enter Your Username : ";

            cin >> name;
            
            //will call UsernameHash() to convert username into encrypted format
            long long hashedUsername = UsernameHash(name);
            
            //will call Existence() function to check whether username exists in our program
            bool found = Existence(hashedUsername);
            
            //if username not found end the Login In terminal
            if(!found)
            {
                cout << endl << "Error. Invalid Username Entered! " << endl << endl;

                cout << endl << "Returning to Main Menu..." << endl << endl;
                
                continue;
            }
            
            //otherwise ask to enter your password
            cout << endl << "Enter Your Password : ";

            cin >> password;
            
            //will call PasswordHash() function to convert password into encrypted format
            long long hashedPassword = PasswordHash(password);
            
            //will call authenticate() function to check whether password correspond to username
            bool auth =  authenticate(name, password);

            if(!auth)
            {
                cout << endl << "Error. Invalid Password Entered! " << endl << endl;

                cout << endl << "Returning to Main Menu..." << endl << endl;
                
                continue;
            }
            
            //successfull login only if username and password are correct
            cout << endl << "Congratulations " << name << "! You Have Been Successfully Logged-In.." << endl << endl;
            
            //Upon entering GB, the program will end otherwise continue to run
            while(input != "GB")
            {
                cout << "What Would You Like to Do? " << endl << endl;

                cout << "1. View Current Friends (VCF)." << endl << endl;  //Enter VCF into terminal

                cout << "2. Add A Friend (AF)." << endl << endl;  //Enter AF into terminal
                
                cout << "3. Remove A Friend (RF)." << endl << endl;  //Enter RF into terminal
 
                cout << "4. View Distinct User Count. (DUC)." << endl << endl;  //Enter DUC into terminal

                cout << "5. Delete Your Account (DA)." << endl << endl;  //Enter DA into terminal

                cout << "6. Your Profile Details (PD). " << endl << endl;  //Enter PD into terminal

                cout << "7. Edit Status (ES). " << endl << endl;  //Enter ES into terminal

                cout << "8. Edit Password (EP). " << endl << endl;  //Enter EP into terminal

                cout << "9. View Friend's Status (VFS). " << endl << endl;  //Enter VFS into terminal

                cout << "10. Go Back (GB). " << endl << endl;  //Enter GB into terminal

                cout << "11. Give us a try to make your day good (RG). " << endl << endl;  //Enter RG into terminal

                //Enter any of given 11 options above
                cin >> input;
                
                //will call random_greeting() function to make your day ٩(ˊᗜˋ*)و ♡
                if(input=="RG")
                {
                 random_greeting();
                }
                
                //operation to list the friends of the user
                else if(input == "VCF")
                {
                    cout << endl << "Displaying Current Friend List: - " << endl << endl;
                    
                    //traverse the graph for the user and print the friends' names from names array 
                    for(long long x = 0 ; x < graph[hashedUsername].size() ; x++)
                        cout << "#" << x + 1 << " : " << names[graph[hashedUsername][x]] << endl << endl;

                    cout << "In Total, You Have " << graph[hashedUsername].size() << " Friends." << endl << endl;

                    system("pause");
                }
                
                //operation to add a friend
                else if(input == "AF")
                {
                    string friendsName;

                    cout << endl << "Enter Your Friend's Name : " << endl << endl;
 
                    //ask for the username of the user you want to add as friend
                    cin >> friendsName;

                    //will convert friendname to encrypted format using UsernameHash() function
                    long long hashedfriendsName = UsernameHash(friendsName);
                    
                    //call Existence() function to check whether your friend have an account in our program
                    bool found = Existence(hashedfriendsName);

                    if(!found)
                    {
                        cout << "Error. Invalid Username Entered! " << endl << endl;

                        cout << "Returning to Main Menu..." << endl << endl;
                        
                        continue;
                    }
                    //if friend have an account, just add it as your friend calling AddFriends()
                    else
                        AddFriends(name,friendsName);

                    system("pause");
                }

                //operation to Remove a friend
                else if(input == "RF")
                { 
                    string friendsName;

                    cout << endl << "Enter Your Friend's Name : " << endl << endl;

                    //ask for the username of the user you want to add as friend
                    cin >> friendsName;

                    //will convert friendname to encrypted format using UsernameHash() function
                    long long hashedfriendsName = UsernameHash(friendsName);

                    //call Existence() function to check whether your friend have an account in our program
                    bool found = Existence(hashedfriendsName);

                    if(!found)
                    {
                        cout << "Error. Invalid Username Entered! " << endl << endl;

                        cout << "Returning to Main Menu..." << endl << endl;
                        
                        continue;
                    }
 
                    //if friend have an account, just remove it from your friendlist calling AddFriends()
                    else 
                        RemoveFriends(name,friendsName);

                    system("pause");
                }

                //operation to get the mutual friends between two users 
                else if(input == "DUC")
                {
                    string friendsName;

                    cout << endl << "Enter Your Friend's Name : " << endl << endl;

                    //ask for the username of the user you want to add as friend
                    cin >> friendsName;

                    //will convert friendname to encrypted format using UsernameHash() function
                    long long hashedfriendsName = UsernameHash(friendsName);

                    //call Existence() function to check whether your friend have an account in our program
                    bool found = Existence(hashedfriendsName);

                    if(!found)
                    {
                        cout << "Error. Invalid Username Entered! " << endl << endl;

                        cout << "Returning to Main Menu..." << endl << endl;
                        
                        continue;
                    }
                    
                    //call DistinctUserCount() function which uses BFS concept
                    else
                        DistinctUserCount(name,friendsName);

                    system("pause");
                }

                //operation to delete your account
                else if(input == "DA")
                {
                    //call function DeleteUser()
                    //We recommend you to not to delete it o(╥﹏╥)o
                    DeleteUser();

                    break;
                }
                
                //operation to fetch your profile details
                else if(input == "PD")
                { 
                    //will display your username, password and status
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
                
                //operation to edit your profile details
                else if(input == "EP")
                {
                    cout << endl << "Enter New Password : ";

                    string pass;
                    //ask new password
                    cin >> pass;
                    
                    //call PasswordHash() to convert pass into encrypted format
                    long long hashedpass = PasswordHash(pass);

                    //update your old password with newpassword
                    hashtable[hashedUsername] = hashedpass;

                    cout << endl << "Your Password Has Been Successfully Changed! " << endl;

                    system("pause");
                }

                //operation to edit your status
                else if(input == "ES")
                {
                    cout << endl << "Initiating Status Updation Procedure... " << endl << endl;

                    //call AddStatus() function where you can add new status
                    string newstatus = AddStatus();
 
                    //update your status with new one
                    status[hashedUsername] = newstatus;

                    cout << endl << "Your Status Has Been Updated!" << endl << endl;

                    system("pause");
                }

                //operation to view your friend's status
                else if(input == "VFS")
                {
                    string friendsName;

                    cout << endl << "Enter Your Friend's Name : " << endl << endl;
                    //enter the friend name whose status you want to see
                    cin >> friendsName;
 
                    //convert friendname into encrypted format
                    long long hashedfriendsName = UsernameHash(friendsName);
 
                    //check whether friend exist in our program
                    bool found = Existence(hashedfriendsName);
                    
                    //if not found, will terminate the operation
                    if(!found)
                    {
                        cout << "Error. Invalid Username Entered! " << endl << endl;

                        cout << "Returning to Main Menu..." << endl << endl;
                        
                        continue;
                    }
                    
                    //iterator to find the friend from your friend list in graph
                    vector<long long> :: iterator it = find(graph[hashedUsername].begin(), graph[hashedUsername].end(), hashedfriendsName);

                    //if iterator points to end+1 then not found, terminate the operation
                    if(it == graph[hashedUsername].end())
                    {
                        cout << "Error. This Person Is Not Your Friend! " << endl << endl;

                        cout << "Returning to Main Menu..." << endl << endl;
                        
                        continue;
                    }
                    
                    //otherwise show the status 
                    cout << endl << friendsName << "'s Status is : - \n\n";

                    cout << status[hashedfriendsName] << endl << endl;

                    system("pause");
                }

                //operation to go back to previous page or operation
                else if(input == "GB")
                    break;
            }
        }
        
        //if not login then operation for sign up will run
        else if(input == "SU")
        {
            cout << endl << "Have a Cappuccino while we're registering you.. ☕︎"<<endl;
            
            //call addUser() function to add you in our program
            addUser();

            system("pause");
        }
    }

    Exit();
    
    return 0;
}
