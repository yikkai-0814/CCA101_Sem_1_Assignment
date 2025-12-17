# include <iostream>
# include <iomanip>
# include <fstream>
# include <string>

using namespace std;

// Declare global variables
const int MAX_USERS = 100;
const int MAX_REVIEWS = 100;

// Functions to read data from files

// Create function to read users.txt
int readUsers(string user_id[], string user_name[], string country[], string state[], 
    string email[], string membership_category[], int current_points[]){
    
    // Create stream object to read users.txt
    ifstream readUserFile;
    readUserFile.open("users.txt");

    // Display appropriate message if the file fails to be opened
    if (readUserFile.fail()){
        cout << "\033[1;91m" << "users.txt failed to be open!" << "\033[0m"; // Display message in red colour and reset it at the end
    }

    // Declare Variable
    int count = 0;

    // Create loop to read data from users.txt into array 
    while (count < MAX_USERS && getline(readUserFile, user_id[count], '\t')) {

        getline(readUserFile, user_name[count], '\t');
        getline(readUserFile, country[count], '\t');
        getline(readUserFile, state[count], '\t');
        getline(readUserFile, email[count], '\t');
        getline(readUserFile, membership_category[count], '\t');

        readUserFile >> current_points[count];
        readUserFile.ignore(); // clear newline

        count++;
    }

    readUserFile.close();
    return count;
}

// Create function to read review.txt
int readReview(string review_user_id[], int review_rating[], string review_statement[], string review_hotel_name[]){
    
    // Create a stream object to read review.txt
    ifstream readReviewFile;
    readReviewFile.open("review.txt");

     // Display appropriate message if the file fails to be opened
    if (readReviewFile.fail()){
        cout << "\033[1;91m" << "review.txt failed to be open!" << "\033[0m"; // Display message in red colour and reset it at the end
    }

    // Declare Variable
    int count_review = 0;
    string temp_review_rating;

    // Create loop to read data from review.txt into array 
    while( count_review < MAX_REVIEWS && getline(readReviewFile, review_user_id[count_review], '\t')){
    
        getline(readReviewFile, temp_review_rating, '\t'); // Read rating as string
        review_rating[count_review] = stoi(temp_review_rating); // Convert the rating from string to int
        getline(readReviewFile, review_statement[count_review], '\t');
        getline(readReviewFile, review_hotel_name[count_review]);

        count_review++;
    }

    readReviewFile.close();
    return count_review;
}

// ------------------------------------------------------------------------------------------------------------

// Functions to update data

// Create function to count number of word of each review
void calculateNumWordReviewStatement(string review_statement[], int number_word_review_statement[], int num_review){

    string text;

    // Create a nested loop to count the number of word
    for (int i=0; i<num_review; i++){

        int count = 1; // Restore the number of word after every loop
        text = review_statement[i];
        
        // Loop to read text 
        for (char temp: text){
            
            if(temp == ' '){
                count++;
            }
        }

        number_word_review_statement[i] = count;
    }
}

// Create function to recalculate points 
int recalculatePoints(string user_id[], string review_user_id[], int current_points[], int number_word_review_statement[], int num_user, 
    int num_review){

    int total_points_awarded = 0;

    // Create nested loop to add points based on number of review made
    for (int i=0; i<num_user; i++){
        
        // Declare variables used to calculate latest points based on number of review
        // Restore variables after every loop
        int count_review= 0;
        int count_review_more_than_50_words = 0;

        for(int j=0; j<num_review; j++){

            if(review_user_id[j] == user_id[i]){

                count_review++; // Update number of review

                if(number_word_review_statement[j] > 50){

                    count_review_more_than_50_words += (number_word_review_statement[j] / 50) ; // Update number of review that has more than 50 words
                }
            }
        }

        current_points[i] += ((count_review * 100) + (count_review_more_than_50_words * 10)); // Update current points based on number of review
        total_points_awarded += ((count_review * 100) + (count_review_more_than_50_words * 10)); // Calculate total points awarded
    }

    return total_points_awarded;
}

// Create function to update membership category
void updateMembershipCategory(string user_id[], string membership_category[], int current_points[], int num_user, string copy_membership_category[], string username[]){
    
    // Initialize boolean status for membership category upgrade status
    bool membership_update = false;

    // Update membership category based on the updated points
    for (int i=0; i<num_user; i++){
        
        if (current_points[i] < 1000){
            membership_category[i] = "Basic";
        }
        else if(current_points[i] >= 1000 && current_points[i] <= 4999){
            membership_category[i] = "Silver";
        }
        else if(current_points[i] >= 5000 && current_points[i] <= 9999){
            membership_category[i] = "Gold";
        }
        else{
            membership_category[i] = "Platinum";
        }

        if(membership_category[i] != copy_membership_category[i]){
            membership_update = true;
        }
    }

     if (membership_update){
        
        cout << "\033[1;96m\n========================== Membership Upgrades ===========================\033[0m" << endl << endl; // Cyan

        cout << "\033[1;93m"; // Bright yellow
        cout << left
             << setw(10) << "UserID"
             << setw(20) << "Username"
             << setw(15) << "Old Category"
             << setw(15) << "New Category"
             << setw(15) << "Current Points" << endl;
        cout << "--------------------------------------------------------------------------" << endl;
        cout << "\033[0m"; // Reset color

        // Print upgraded members in green
        for (int i = 0; i < num_user; i++) {
            if (membership_category[i] != copy_membership_category[i]) {
                cout << left
                     << setw(10) << user_id[i]
                     << setw(20) << username[i]
                     << setw(15) << copy_membership_category[i]
                     << setw(15) << membership_category[i]
                     << setw(15) << current_points[i] << endl;
            }
        }
    }

}

// Create function to calculate number of hotel and Record name of hotel uniquely
int calculateNumHotel(string review_hotel_name[], int num_review, string hotel_name[]){

    // Initialize number of hotel
    int number_hotel = 0;

    for (int i=0; i<num_review; i++){

        // Initialize and restore boolean status after every loop
        bool found_status = false;

        for (int j=0; j<number_hotel; j++){

            if(hotel_name[j] == review_hotel_name[i]){

                found_status = true;
                break;
                
            }
        }

        if (found_status == false){
            
            hotel_name[number_hotel] = review_hotel_name[i];
            number_hotel++;

        }

    }

    return number_hotel;
}

// Create function to calculate number of review made on a hotel and total rating of a hotel
void calculateNumReviewHotel(string review_hotel_name[], int review_rating[], int num_review, int number_hotel, string hotel_name[], 
    int number_review_hotel[], int total_rating_hotel[]){

    for (int i=0; i<number_hotel; i++){

        number_review_hotel[i] = 0;
        total_rating_hotel[i] = 0;

        for (int j=0; j<num_review; j++){
            
            if (review_hotel_name[j] == hotel_name[i]){
                
                number_review_hotel[i]++;
                total_rating_hotel[i] += review_rating[j];

            }
        }
    }
}

// -----------------------------------------------------------------------------------------------------------------------------

// Functions used for output features 

// Create function to find username based on review user id
string findUsername(int num_user, string user_name[], string review_user_id, string user_id[], string username){

    for (int i=0; i<num_user; i++){

        if (review_user_id == user_id[i]){
            username = user_name[i];
            break;
        }
    }

    return username;
}

// Create function to display review made by a specific user
void displaySpecificUserReview(string user_id[], string user_name[], string review_user_id[], int review_rating[], 
    string review_statement[],string review_hotel_name[], string input_user_id, int num_review, int num_user, int choice){

    // Initialize boolean status
    bool review_status = false;
    bool user_status = false;

    // Loop to determine whether the entered userID is in the user list or not
    for (int i=0; i<num_user; i++){

        if (input_user_id == user_id[i]){
            user_status = true;
        }

        for (int j=0; j<num_review; j++){

            if (input_user_id == review_user_id[j]){
                review_status = true;
            }
        }
    }

    if (user_status == true && review_status == true){

        string username = " "; // Initialize username
        int count = 1;

        // Loop to find the review based on review user id
        for (int i=0 ; i< num_review; i++){

            // Call function to find username based on review id
            username = findUsername(num_user, user_name, review_user_id[i], user_id, username);

            if (input_user_id == review_user_id[i]){

                 // Display Header
                cout << "\033[1;96m"<< "\n================================\n";
                cout << "         User Review " << count << endl;
                cout << "================================\n" << "\033[0m";

                cout << left;
                cout << setw(15) << "User ID" << ": " << review_user_id[i] << endl;
                cout << setw(15) << "Username" << ": " << username << endl;
                cout << setw(15) << "Rating" << ": "<<review_rating[i] << "/5\n";
                cout << setw(15) << "Hotel Name" << ": " << review_hotel_name[i] << endl;
                cout << setw(15) << "Statement" << ": " << review_statement[i] << endl;
                count++;
            }
        }
    }

    // Display appropriate message when the user is not found
    if (user_status == false){
        cout << "\033[1;91m" << "User is not found!\n" << "\033[0m";  // Display message in red color and reset it at the end
    }
    else{ 
        // Display appropriate message when no review is made by the user
        if (review_status == false){
            cout << "\033[1;91m" << "This user doesn't make any review.\n" << "\033[0m"; // Display message in red color and reset it at the end
        }
    }
}

// Create function to display review for a specific hotel
void displaySpecificHotelReview(string input_hotel_name, string review_hotel_name[], string review_user_id[], int review_rating[], 
    string review_statement[], string user_name[], string user_id[], int number_review, int num_user, int number_hotel, string hotel_name[]){

    // Declare boolean status
    bool review_status = false;
    bool hotel_status = false;

    for (int i=0; i<number_review; i++){

        for (int j=0; j<number_hotel; j++){

            if (input_hotel_name == hotel_name[j]){
                hotel_status = true;
            }
        }

        if (input_hotel_name == review_hotel_name[i]){
            review_status = true;
        }
    }

    if (review_status == true && hotel_status == true){

        string username = " "; // Initialize username

        int count = 1;

        // Create loop to display the review of a specific hotel
        for (int i=0 ;i<number_review; i++){

            // Call function to find username based on review user id
            username = findUsername(num_user, user_name, review_user_id[i], user_id, username);

            if (input_hotel_name == review_hotel_name[i]){

                // Display Header
                cout << "\033[1;96m"<< "\n================================\n";
                cout << "         Hotel Review " << count << endl;
                cout << "================================\n" << "\033[0m";

                cout << left;
                cout << setw(15) << "User ID" << ": " << review_user_id[i] << endl;
                cout << setw(15) << "Username" << ": " << username << endl;
                cout << setw(15) << "Rating" << ": "<<review_rating[i] << "/5\n";
                cout << setw(15) << "Statement" << ": " << review_statement[i] << endl;
                count++;

                review_status = true;
            }   
        }
    }
    
    // Display appropriate message when the input hotel name is not in the list of hotel
    if (hotel_status == false){
        cout << "\033[1;91m" << "Hotel is not found! Please use the hotel name listed above!\n" << "\033[0m" ; // Display message in red color and reset it at the end
    }
    else{
        if(review_status == false){
            cout << "\033[1;91m" << "No review is made to this hotel!\n" << "\033[0m"; // Display message in red color and reset it at the end
        }
    }
}

// Create function to display the latest user information
void displayUserLatestInfo(string user_id[], string user_name[], string country[], string state[], string email[], string membership_category[], 
    int current_points[] , int num_user){

    // Display Header
    cout << "\033[1;94m"; // Blue colour
    cout << left 
    << setw(10) << "UserID" 
    << setw(20) <<"Username" 
    << setw(15) << "Country" 
    << setw(18) << "State" 
    << setw(30) << "Email Address" 
    << setw(25) << "Membership Category" 
    << setw(12) << "Current Points" << endl;
    cout << "\033[0m"; // Restore colours to default

    // Create loop to display the latest user information
    for (int i=0; i<num_user; i++){
        cout << left 
        << setw(10) << user_id[i] 
        << setw(20) << user_name[i] 
        << setw(15) << country[i] 
        << setw(18) << state[i] 
        << setw(30) << email[i] 
        << setw(25) << membership_category[i]
        << setw(12) << current_points[i] << endl;
    }
}

// Create function to display review of high rating 4 or 5 for specific hotel 
void displayHighRatingReview(string input_hotel_name, string review_hotel_name[], string review_user_id[], int review_rating[], string review_statement[], 
    string user_id[], string user_name[], int num_user, int num_review){
    

     // Initialize boolean status
    bool review_status = false;

    for (int i=0; i<num_review; i++){

        if(input_hotel_name == review_hotel_name[i]){
            
            if (review_rating[i] >= 4){
                
                review_status = true;
            }
        }
    }

    if (review_status == true){

        string username = " "; // Initialize username
        int count = 1;

        // Create loop to display review of a specific hotel
        for (int i=0; i<num_review; i++){

            // Call function to find username based on review user id
            username = findUsername(num_user, user_name, review_user_id[i], user_id, username);
            
            if(input_hotel_name == review_hotel_name[i]){
                
                if (review_rating[i] >= 4){
                    
                    // Display Header
                    cout << "\033[1;96m"<< "\n================================\n";
                    cout << "         Hotel Review " << count << endl;
                    cout << "================================\n" << "\033[0m";

                    cout << left;
                    cout << setw(15) << "User ID" << ": " << review_user_id[i] << endl;
                    cout << setw(15) << "Username" << ": " << username << endl;
                    cout << setw(15) << "Rating" << ": "<<review_rating[i] << "/5\n";
                    cout << setw(15) << "Statement" << ": " << review_statement[i] << endl;
                    count++;
                }
            }
        }
    }
    else{ // Display appropriate message if there is no review with rating 4 or 5 for this hotel
        cout << "There is no high rating review for " << input_hotel_name << ".\n";
    }
}

// Create function to display summary of activities
void displaySummaryActivities(int num_user, int num_review, int total_points_awarded){
    cout << "Total number of users: " << num_user << endl;
    cout << "Total number of reviews: " << num_review << endl;
    cout << "Total points awarded system-wide: "<< total_points_awarded << endl;
}

// Create function that used for display specific membership category members
void innerDisplayUsersByMembership(string user_id[], string user_name[], string membership_category[], int current_points[], int num_user, string target_category){

    // Declare boolean status
    bool membership_status = false;

    for (int i=0 ;i<num_user; i++){

        if (membership_category[i] == target_category){
            membership_status = true;
        }
    }

    if (membership_status == true){
        // Display Header
        cout << endl << target_category << " Member\n";
        cout << "\033[1;96m"; // Cyan 
        cout << left
        << setw(10) << "UserID"
        << setw(25) << "Username"
        << setw(12) << "Current Points"
        << endl;
        cout << "\033[0m"; // Reset Colours

        // Loop to find target category member
        for (int i=0; i<num_user; i++){

            if (membership_category[i] == target_category){
                cout << left
                << setw(10) << user_id[i]
                << setw(25) << user_name[i]
                << setw(12) << current_points[i]
                << endl;
            }
        }
    }
    else{
        cout << endl << target_category << " Member\n";
        cout << "\033[1;91m" << "No user is found in this membership category.\n" << "\033[0m"; // Dispaly message in red colour and reset it at the end
    }
}

// Create function to display users grouped by membership category
void displayUsersByMembership(string user_id[], string user_name[], string membership_category[], int current_points[], int num_user){

    int choice;
    bool choice_status = false;
    
    while(choice_status == false){ // Input Data Validation
        cout << "\033[1;95m" << "\n===========================\n" << "\033[0m";  // Display text in purple and restore it at the end
        cout << "\033[1;94m" << "   Sorting Features Menu\n" << "\033[0m";     // Display text in blue and restore it at the end
        cout << "\033[1;95m" << "===========================\n" << "\033[0m";
        cout << "\033[1;94m" << "1. Ascending Order\n2. Descending Order\n" << "\033[0m";
        cout << "\nEnter your choice: ";
        cin >> choice;
        cin.ignore(); // flush the buffer

        if (choice == 1 || choice == 2){
            choice_status = true;
        }
        else{
            cout << "\033[1;91m" << "\nInvalid Choice! Please make input again!\n";
        }
    }

    // Sort users in ascending order
    if (choice == 1){ 
        for (int i=0;i<num_user -1; i++){
            for (int j=i+1; j<num_user; j++){
                if (current_points[j] < current_points[i]){
                    swap(user_id[j], user_id[i]);
                    swap(user_name[j], user_name[i]);
                    swap(current_points[j], current_points[i]);
                    swap(membership_category[j], membership_category[i]);
                }
            }
        }
    }
    // Sort users in descending order
    else if (choice == 2){ 
        for (int i=0; i<num_user -1; i++){
            for (int j=i+1; j<num_user; j++){
                if (current_points[j] > current_points[i]){
                    
                    swap(user_id[j], user_id[i]);
                    swap(user_name[j], user_name[i]);
                    swap(current_points[j], current_points[i]);
                    swap(membership_category[j], membership_category[i]);
                }
            }
        }
    }


    // Call function to display platinum member
    innerDisplayUsersByMembership(user_id, user_name, membership_category, current_points, num_user, "Platinum");

    // Call function to display Gold member
    innerDisplayUsersByMembership(user_id, user_name, membership_category, current_points, num_user, "Gold");

    // Call function to display Silver member
    innerDisplayUsersByMembership(user_id, user_name, membership_category, current_points, num_user, "Silver");

    // Call function to display Basic member
    innerDisplayUsersByMembership(user_id, user_name, membership_category, current_points, num_user, "Basic");

}

// Create function to sort users based on number of review
void sortUsersNumReview(int num_user, int count_review[], string user_id[], string user_name[]){

    // Create nested loop to sort the user into descending order based on the number of review made 
    for (int i=0; i<num_user - 1; i++){

        for (int j=i+1 ; j<num_user; j++){

            if (count_review[j] > count_review[i]){

                // Swap position of array
                swap(user_id[j], user_id[i]);
                swap(user_name[j], user_name[i]);
                swap(count_review[j], count_review[i]);
            }

        }
    }
}

// Create function to display top reviewer leaderboard
void displayTopReviewerLeaderboard(int num_user, int count_review[], string user_id[], string user_name[], int current_points[]){

    // Display Header
    cout << "\033[1;96m"; // Cyan
    cout << left 
    << setw(15) << "Rankings"
    << setw(10) << "UserID"
    << setw(20) << "Username"
    << setw(10) << "Number of Review Made"
    << endl;
    cout << "\033[0m"; // Reset Colour
    
    for (int i=0; i<num_user; i++){
        
        // Calculate number of review of each user based on their current points and assume all review is not more than 50 words
        count_review[i] = current_points[i] / 100;
    }


    // Call Function
    sortUsersNumReview(num_user, count_review, user_id, user_name);

    // Initialize rank
    int rank = 1;

    // Display user information based on their rankings
    for (int i=0; i<num_user; i++){

        // if same count as previous → same rank
        if (i > 0 && count_review[i] == count_review[i - 1]) {
            // rank stays the same
        } 
        else {
            // new rank = current index + 1
            rank = i + 1;
        }

        cout << left
        << setw(15) << rank
        << setw(10) << user_id[i]
        << setw(20) << user_name[i]
        << setw(10) << count_review[i]
        << endl; 
    }

}

// Create function to display top 3 users with most reviews
void displayTop3Reviewer(int num_user, int count_review[], string user_id[], string user_name[], int current_points[]){
    
    for (int i=0; i<num_user; i++){
        
        // Calculate number of review of each user based on their current points and assume all review is not more than 50 words
        count_review[i] = current_points[i] / 100;
    }

    // Display Header
    cout << "\033[1;96m"; // Cyan
    cout << left 
    << setw(15) << "Rankings"
    << setw(10) << "UserID"
    << setw(20) << "Username"
    << setw(10) << "Number of Review Made"
    << endl;
    cout << "\033[0m"; // Reset Colour

    // Call Function
   sortUsersNumReview(num_user, count_review, user_id, user_name);

    // Initialize rank
    int rank = 1;

    for (int i=0; i<num_user; i++){

        // if same count as previous → same rank
        if (i > 0 && count_review[i] == count_review[i - 1]) {
            // rank stays the same
        } 
        else {
            // new rank = current index + 1
            rank = i + 1;
        }
        
        if (rank <= 3){
            cout << left
            << setw(15) << rank
            << setw(10) << user_id[i]
            << setw(20) << user_name[i]
            << setw(10) << count_review[i]
            << endl;
        }
    }
}

// Create function to display hotel rating summary
void displayHotelRatingSummary(int num_hotel, string hotel_name[], int number_review_hotel[], int total_rating_hotel[]){

    int choice;
    bool choice_status = false;

    while(choice_status == false){ // Input Data Validation
        cout << "\033[1;95m" << "\n===========================\n" << "\033[0m";  // Display text in purple and restore it at the end
        cout << "\033[1;94m" << "   Sorting Features Menu\n" << "\033[0m";     // Display text in blue and restore it at the end
        cout << "\033[1;95m" << "===========================\n" << "\033[0m";
        cout << "\033[1;94m" << "1. Ascending Order\n2. Descending Order\n" << "\033[0m";
        cout << "\nEnter your choice: ";
        cin >> choice;
        cin.ignore(); // flush the buffer

        if (choice == 1 || choice == 2){
        choice_status = true;
        }
        else{
            cout << "\033[1;91m" << "\nInvalid Choice! Please make input again!\n";
        }
    }

    // Sort hotel in ascending order 
    if (choice == 1){
        for (int i=0; i< num_hotel-1; i++){
            for (int j=i+1; j<num_hotel; j++){
                if (number_review_hotel[j] < number_review_hotel[i]){
                    swap (hotel_name[j], hotel_name[i]);
                    swap (number_review_hotel[j], number_review_hotel[i]);
                    swap (total_rating_hotel[j], total_rating_hotel[i]);
                }
            }
        }
    }
    // Sort hotel in descending order
    else if(choice == 2){
        for (int i=0; i< num_hotel-1; i++){
            for (int j=i+1; j<num_hotel; j++){
                if (number_review_hotel[j] > number_review_hotel[i]){
                    swap (hotel_name[j], hotel_name[i]);
                    swap (number_review_hotel[j], number_review_hotel[i]);
                    swap (total_rating_hotel[j], total_rating_hotel[i]);
                }
            }
        }
    }

    // Dislay Header
    cout << "\033[1;96m";
    cout << endl;
    cout << left 
    << setw(30) << "Hotel Name"
    << setw(20) << "Average Rating"
    << setw(20) << "Number of Review"
    << endl;
    cout << "\033[0m"; // Restore colour to default at the end

    for (int i=0; i<num_hotel; i++){

        double average_rating = static_cast<double>(total_rating_hotel[i]) / static_cast<double>(number_review_hotel[i]);

        cout << left 
        << setw(30) << hotel_name[i]
        << setw(20) << fixed << showpoint << setprecision(2) << average_rating
        << setw(20) << number_review_hotel[i]
        << endl;

    }
}

// Create function add new review and return new number of review
int addNewReview(int num_review, int num_user, string review_user_id[], string user_id[], int current_points[], string review_hotel_name[], 
    string review_statement[], int review_rating[], string hotel_name[], int number_review_hotel[], int total_rating_hotel[]){

    bool user_status = false;

    string input_user_id;
    string input_hotel_name;
    string input_statement;
    int input_rating;

    cout << "Enter UserID: ";
    cin >> input_user_id;
    cin.ignore(); // Clear the buffer

    cout << "Enter Hotel Name: ";
    getline(cin, input_hotel_name);

    cout << "Enter Review Statement: ";
    getline(cin, input_statement);

    cout << "Enter Rating: ";
    cin >> input_rating;
    cin.ignore(); // Clear the buffer

    // Input Data Validation
    while (input_rating < 0 || input_rating > 5){
        cout << "\033[1;91m" << "Invalid Input for Rating.\nPlease make your input again!\n\n" << "\033[0m"; // Display message in red colours and reset it at the end
        cout << "Enter Rating: ";
        cin >> input_rating;
        cin.ignore(); // Clear the buffer
    }


    // Determine whether the input user id is exist in the user list 
    for (int i=0; i<num_user ; i++){

        if (input_user_id == user_id[i]){
            user_status = true;
            break;
        }
    }

    // Input Validation
    while (user_status != true){

        cout << "\033[1;91m" << "\nUser is not found in the user list!\nPlease make your input again!\n\n" << "\033[0m"; // Display message in red colour and reset it at the end
        
        cout << "Enter UserID: ";
        cin >> input_user_id;
        cin.ignore(); // Clear the buffer

        cout << "Enter Hotel Name: ";
        getline(cin, input_hotel_name);

        cout << "Enter Review Statement: ";
        getline(cin, input_statement);

        cout << "Enter Rating: ";
        cin >> input_rating;
        cin.ignore(); // Clear the buffer

        // Input Data Validation
        while (input_rating < 0 || input_rating > 5){
            cout << "Invalid Input for Rating.\n";
            cout << "Enter Rating: ";
            cin >> input_rating;
            cin.ignore(); // Clear the buffer
        }

        // Determine whether the input user id is exist in the user list 
        for (int i=0; i<num_user ; i++){

            if (input_user_id == user_id[i]){
                user_status = true;
                break;
            }
        }
    }

    // Read the entered data into array if the user is found in the user list
    if (user_status == true){

        review_user_id[num_review] = input_user_id;
        review_hotel_name[num_review] = input_hotel_name;
        review_statement[num_review] = input_statement;
        review_rating[num_review] = input_rating;
        num_review++;
    }

    int count = 1;

    // Count the number of character of the review statement made
    for (char temp: input_statement){
            
            if(temp == ' '){
                count++;
            }

    }

    // Update the current point and membership category
    for (int i=0; i<num_user; i++){

        if (input_user_id == user_id[i]){
            
            // Update current points of the user once a new review is made
            current_points[i] += 100;

            if (count >= 50){
                current_points[i] += (count / 50) * 10;
            }
        }
    }

    // Call function to update the number of hotel and record if there is new hotel name
    int number_hotel = calculateNumHotel(review_hotel_name, num_review, hotel_name);

    // Call function to update number of review of each hotel
    calculateNumReviewHotel(review_hotel_name, review_rating, num_review, number_hotel, hotel_name, number_review_hotel, total_rating_hotel);

    return number_hotel;
}

// Create function to update users.txt
void updateUserFile(int num_user, string user_id[], string user_name[], string country[], string state[], string email[], string membership_category[], int current_points[]){
    
    ofstream updateUserFile; // Create a output file stream object for users.txt
    updateUserFile.open("users.txt"); // Open file to clear the existing data and input data to users.txt

    // Update the latest user data into users.txt
    for (int i=0; i<num_user; i++){

        updateUserFile << user_id[i] << "\t" 
        << user_name[i] << "\t" 
        << country[i] << "\t" 
        << state[i] << "\t" 
        << email[i] << "\t" 
        << membership_category[i] << "\t" 
        << current_points[i] << endl;

    }

    // Display appropriate message to tell user whether data is saved to the file
    if (updateUserFile.is_open()){
        cout << "\033[1;95m" << "\nLatest User Information have been updated to users.txt!" << "\033[0m"; // Display message in purple and reset it at the end
    }
    else{
        cout << "\033[1;91m" << "\nData fails to be updated to users.txt!" << "\033[0m"; // Display message in red and reset it at the end
    }

    // Close the output file stream object 
    updateUserFile.close();
}

// Create function to update review.txt
void updateReviewFile(int num_review, string review_user_id[], int review_rating[], string review_statement[], string review_hotel_name[]){
    
    ofstream updateReviewFile; // Create a output file stream object for review.txt
    updateReviewFile.open("review.txt"); // Open file to clear the existing data and input data to review.txt

    // Update latest review data into review.txt
    for(int i=0; i<num_review; i++){
        
        updateReviewFile << review_user_id[i] << "\t"
        << review_rating[i] << "\t"
        << review_statement[i] << "\t"
        << review_hotel_name[i] << endl;
    }

    // Display appropriate message to tell user whether the data is saved into the file
    if (updateReviewFile.is_open()){
        cout << "\033[1;95m" << "\nThe latest review have been updated to review.txt!" << "\033[0m"; // Display message in purple and reset it at the end
    }
    else{
        cout << "\033[1;91m" << "\nData fails to be updated to review.txt!" << "\033[0m"; // Display message in red and reset it at the end
    }

    // Close the output file stream object 
    updateReviewFile.close();
}


int main(){

    // Create array for users.txt file
    string user_id[MAX_USERS], 
    user_name[MAX_USERS], 
    country[MAX_USERS], 
    state[MAX_USERS], 
    email[MAX_USERS], 
    membership_category[MAX_USERS];

    int current_points[MAX_USERS], 
    count_review[MAX_USERS];

    // Create array for review.txt
    string review_user_id[MAX_REVIEWS], 
    review_statement[MAX_REVIEWS], 
    review_hotel_name[MAX_REVIEWS], 
    hotel_name[MAX_REVIEWS];

    int review_rating [MAX_REVIEWS], 
    number_word_review_statement[MAX_REVIEWS], 
    number_review_hotel[MAX_REVIEWS], 
    total_rating_hotel[MAX_REVIEWS];
    
    // Call readUsers function to read data into array and find the number of users
    int num_user = readUsers(user_id, user_name, country, state, email, membership_category, current_points);

    // Call readReview function to read data into array and find the number of reviews
    int num_review = readReview(review_user_id, review_rating, review_statement, review_hotel_name);

    // Call function to find the number of word of each review
    calculateNumWordReviewStatement(review_statement, number_word_review_statement,num_review);

    // Call function to recalculate current points
    int total_points_awarded = recalculatePoints(user_id, review_user_id, current_points, number_word_review_statement, num_user, num_review);

    // Copy array for membership uprade notification purpose
    string copy_membership_category[MAX_USERS];

    // Loop to copy the membership category
    for (int i=0; i<num_user; i++){
        copy_membership_category[i] = membership_category[i];
    }

    // Call function to update membership category
    updateMembershipCategory(user_id, membership_category, current_points, num_user, copy_membership_category, user_name);

    // Call function to find the number of hotel & record the unique hotel name
    int number_hotel = calculateNumHotel(review_hotel_name, num_review, hotel_name);

    // Call function to calculate number of review and rating of a hotel
    calculateNumReviewHotel(review_hotel_name, review_rating, num_review, number_hotel, hotel_name, number_review_hotel, total_rating_hotel);

    // Copy array for sorting purpose
    string copy_user_id[MAX_USERS], 
    copy_user_name[MAX_USERS],
    sort_membership_category[MAX_USERS],
    copy_hotel_name[MAX_USERS];

    int copy_current_points[MAX_USERS],
    copy_number_review_hotel[MAX_REVIEWS],
    copy_total_rating_hotel[MAX_REVIEWS];

    // Create menu-driven interface
    // Declare variables used for output features
    int choice = 0; 
    string input_user_id , input_hotel_name;

    // Data Validation
    while (choice != 11){

        // Display Header
        cout << "\033[1;95m\n====================== TRAVELGO MAIN MENU ======================\033[0m" << endl;
        cout << "\033[1;94m[1]\033[0m  Display User Reviews" << endl;
        cout << "\033[1;94m[2]\033[0m  Display Hotel Reviews" << endl;
        cout << "\033[1;94m[3]\033[0m  User Information" << endl;
        cout << "\033[1;94m[4]\033[0m  Positive Hotel Reviews" << endl;
        cout << "\033[1;94m[5]\033[0m  Activity Summary" << endl;
        cout << "\033[1;94m[6]\033[0m  Membership Categories" << endl;
        cout << "\033[1;94m[7]\033[0m  Reviewer Leaderboard" << endl;
        cout << "\033[1;94m[8]\033[0m  Top 3 Reviewers" << endl;
        cout << "\033[1;94m[9]\033[0m  Hotel Rating Summary" << endl;
        cout << "\033[1;94m[10]\033[0m Add Review" << endl;
        cout << "\033[1;94m[11]\033[0m Save & Exit" << endl;
        cout << "\033[1;95m================================================================\033[0m" << endl;

        cout << "\nEnter your choice: ";
        cin >> choice; 
        cin.ignore(); // there is a newline after the choice entered so we need to flush the buffer
    
        // Input Data Validation
        while (choice > 12){
            cout << "\nInvalid Choice!\n";
            cout << "Enter your choice: ";
            cin >> choice; 
            cin.ignore(); // there is a newline after the choice entered so we need to flush the buffer
        }

        switch(choice){

            // Display All Reviews Written by a Specific User
            case 1:
                cout << "Enter User ID: "; // Prompt user to make input
                cin >> input_user_id;
                cin.ignore();

                // Call function
                displaySpecificUserReview(user_id, user_name, review_user_id, review_rating, review_statement, review_hotel_name, input_user_id, num_review, num_user, choice);
                break;

            // Display All Reviews for a specific hotel 
            case 2:

                // Display Available Hotel 
                cout << endl << "\033[1;91m" <<  "Available Hotel List: " << "\033[0m" << endl;
                for (int i=0; i<number_hotel; i++){
                    cout << "\033[1;94m" << "[" <<  i + 1 << "] "  << hotel_name[i] << "\033[0m" << endl;
                }

                cout << endl << "Enter the name of hotel: "; // Prompt user to make input
                getline(cin, input_hotel_name);

                // Call Function
                displaySpecificHotelReview(input_hotel_name, review_hotel_name, review_user_id, review_rating, review_statement, user_name, user_id, num_review, num_user, number_hotel, hotel_name);
                break;

            // Display updated user information
            case 3:
                cout << endl;

                // Call function
                displayUserLatestInfo(user_id, user_name, country, state, email, membership_category, current_points, num_user);
                break;

            // Display All reviews with rating 4 or 5 for a specific hotel
            case 4:

                // Display Available Hotel 
                cout << endl << "\033[1;91m" <<  "Available Hotel List: " << "\033[0m" << endl;
                for (int i=0; i<number_hotel; i++){
                    cout << "\033[1;94m" << "[" <<  i + 1 << "] "  << hotel_name[i] << "\033[0m" << endl;
                }
                
                cout << endl << "Enter the name of hotel: ";
                getline(cin, input_hotel_name);

                // Call Function
                displayHighRatingReview(input_hotel_name, review_hotel_name, review_user_id, review_rating, review_statement, user_id, user_name, num_user, num_review);
                break;

            // Display Summary of Activities
            case 5:
                cout << endl; 

                // Call Function
                displaySummaryActivities(num_user, num_review, total_points_awarded);
                break;

            // Display users grouped by membership category
            case 6:
                
                // Loop to copy array
                for (int i=0; i<num_user; i++){
                    copy_user_id[i] = user_id[i];
                    copy_user_name[i] = user_name[i];
                    copy_current_points[i] = current_points[i];
                    sort_membership_category[i] = membership_category[i];
                }

                // Call Function
                displayUsersByMembership(copy_user_id, copy_user_name, sort_membership_category, copy_current_points, num_user);
                break;

            // Display Top Reviewer Leaderboard
            case 7:

                // Loop to copy array
                for (int i=0; i<num_user; i++){
                    copy_user_id[i] = user_id[i];
                    copy_user_name[i] = user_name[i];
                    copy_current_points[i] = current_points[i];
                }

                cout << endl;

                // Call function
                displayTopReviewerLeaderboard(num_user, count_review, copy_user_id, copy_user_name, copy_current_points);
                break;

            // Display Top 3 Users with most reviews
            case 8:

                // Loop to copy array
                for (int i=0; i<num_user; i++){
                    copy_user_id[i] = user_id[i];
                    copy_user_name[i] = user_name[i];
                    copy_current_points[i] = current_points[i];
                }

                cout << endl;

                // Call Function
                displayTop3Reviewer(num_user, count_review, copy_user_id, copy_user_name, copy_current_points);
                break;

            // Display Hotel Rating Summary
            case 9:
                cout << endl;

                // Copy array for sorting purpose
                for (int i=0; i<number_hotel; i++){
                    copy_hotel_name[i] = hotel_name[i];
                    copy_number_review_hotel[i] = number_review_hotel[i];
                    copy_total_rating_hotel[i] = total_rating_hotel[i];
                }

                // Call Function
                displayHotelRatingSummary(number_hotel, copy_hotel_name, copy_number_review_hotel, copy_total_rating_hotel);
                break;

            // Add new review
            case 10:
                cout << endl;

                // Call Function
                number_hotel = addNewReview(num_review, num_user, review_user_id, user_id, current_points, review_hotel_name, review_statement, review_rating, hotel_name, number_review_hotel, total_rating_hotel);

                // Update number of review
                num_review++;

                // Update the total points awarded
                int count = 1;
                // Count the number of character of the review statement made
                for (char temp: review_statement[num_review]){
                        
                        if(temp == ' '){
                            count++;
                        }

                }
                if (count >= 50){
                    total_points_awarded += ( 100 + ((count / 50) * 10) );
                }
                else{
                    total_points_awarded += 100;
                }

                // Loop to copy the membership category
                for (int i=0; i<num_user; i++){
                    copy_membership_category[i] = membership_category[i];
                }

                // Call function to update the membership category based on the latest points
                updateMembershipCategory(user_id, membership_category, current_points, num_user, copy_membership_category, user_name);
                break;

                // Call Function to perform Additional Features
        }       
    }

    // Call function to update users.txt
    updateUserFile(num_user, user_id, user_name, country, state, email, membership_category, current_points);

    // Call function to update review.txt
    updateReviewFile(num_review, review_user_id, review_rating, review_statement, review_hotel_name);

    cout << "\033[1;96m" << "\n\nThank you for using this system!" << "\033[0m"; // Display message in cyan and reset it at the end
    return 0;
}