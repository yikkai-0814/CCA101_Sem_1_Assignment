int readUsers(string user_id[], string user_name[], string country[], string state[], 
    string email[], string membership_category[], int current_points[]){
    
    // Create stream object to read users.txt
    ifstream inFile;
    inFile.open("users.txt");

    // Declare Variable
    int count = 0;

    // Create loop to read data from users.txt into array 
    while (getline(inFile, user_id[count], '\t')) {

        getline(inFile, user_name[count], '\t');
        getline(inFile, country[count], '\t');
        getline(inFile, state[count], '\t');
        getline(inFile, email[count], '\t');
        getline(inFile, membership_category[count], '\t');

        inFile >> current_points[count];
        inFile.ignore(); // clear newline

        count++;
    }

    inFile.close();
    return count;
}