for (int i=0 ; i< num_review; i++){

            // Call function to find username based on review id
            username = findUsername(num_user, user_name, review_user_id[i], user_id, username);

            if (input_user_id == review_user_id[i]){

                // Display Header
                cout << "\033[1;96m"<< "=============================\n";
                cout << setw(20) << "User Review " << count << endl;
                cout << "=============================\n" << "\033[0m";

                cout << left;
                cout << setw(15) << "User ID" << ": " << review_user_id[i] << endl;
                cout << setw(15) << "Username" << ": " << username << endl;
                cout << setw(15) << "Rating" << ": "<<review_rating[i] << "/5\n";
                cout << setw(15) << "Hotel Name" << ": " << review_hotel_name[i] << endl;
                cout << setw(15) << "Statement" << ": " << review_statement[i] << endl << endl;
            }

            count++;
        }