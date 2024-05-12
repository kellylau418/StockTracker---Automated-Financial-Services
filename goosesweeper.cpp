#include "geesespotter_lib.h"
#include <iostream>
//g++ -std=c++11 geesespotter.cpp geesespotter_lib.cpp -o main
// ./main
char *create_board(std::size_t x_dim, std::size_t y_dim){
    char *board{ new char[x_dim * y_dim] };

    for (std::size_t k{0}; k < (x_dim * y_dim); ++k){

        board[k] = 0;
        //std::cout << k << ": " <<  board[k]  << std::endl;
    }

    //std::cout << "Create board" << board << std::endl;
    return board;

}
void clean_board(char *board){
    
    delete[] board;
    board = nullptr;


}
void print_board(char *board, std::size_t x_dim, std::size_t y_dim){
    //print board content
    //loop over every y
    //board[1] = 0x33;
    //board[7] = 0x4;


    //std::cout << "Testing board items: " << std::hex << static_cast<int>(board[1]) << std::endl;
    //std::cout << "Testing board items: " << std::hex << static_cast<int>(board[10]) << std::endl;
   
    for ( std::size_t k1{0}; k1 < y_dim; ++k1 ){
        // in every every, loop over every x

        for( std::size_t k2{0}; k2 < x_dim; ++k2 ){
            //see if the 1st hex number is 0, 2, or 3 and print accordinly

            if((board[k2 + k1*x_dim] & hidden_mask()) && (board[k2 + k1*x_dim] & marked_mask())){
                //field is marked, print M
                std::cout << "M";

            }else if(board[k2 + k1*x_dim] & hidden_mask()){
                //field is hidden, print *
                std::cout << "*";
            }else{
                //field is revealed, print the num rep by the 1st 4 bits of char
                std::cout << int(board[k2 + k1*x_dim]);
            }

        }

        std::cout << std::endl;
    

    }
    std::cout << std::endl;
        
   

}

void hide_board(char *board, std::size_t x_dim, std::size_t y_dim){
    for (std::size_t k1{0}; k1 < y_dim; ++k1){
        for (std::size_t k2{0}; k2 < x_dim; ++k2){
            board[k2 + k1*x_dim] |= hidden_mask();
        }
    }

}

int mark(char *board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc, std::size_t y_loc){
    //if field is revealed, return 2
    //if field is hidden, make it marked and return 0
    //if field is marked already, tun it off and return 0
    //board[1] = 0x33;
    //board[7] = 0x4;

    if((board[x_loc + y_loc*x_dim] & hidden_mask()) && (board[x_loc + y_loc*x_dim] & marked_mask())){
        //field is marked
        board[x_loc + y_loc*x_dim] &= hidden_mask();
        return 0;

    }else if (board[x_loc + y_loc*x_dim] & hidden_mask()){
        //field is hidden
        board[x_loc + y_loc*x_dim] |= marked_mask();
        return 0;

    }else{   
        return 2;
    }
}

void compute_neighbours(char *board, std::size_t x_dim, std::size_t y_dim){
    //loop throught every y_dim them x_dim
    for (std::size_t k1{0}; k1 < y_dim; ++k1){
        for (std::size_t k2{0}; k2 < x_dim; ++k2){
            bool left_num{false};
            bool right_num{false};
            bool top_num{false};
            bool bottom_num{false};
            int goose_count{};

            if (board[k2 + k1*x_dim] != 9){
            
                //1. does num on left exist
                if ( k2 != 0){
                    left_num = true;
                    if (board[k2 - 1 + k1*x_dim] == 9){
                        goose_count += 1;
                    }
                }
                //2. does num on right exist
                if ( k2 != (x_dim -1)){
                    right_num = true;
                    if (board[k2 + 1 + k1*x_dim] == 9){
                        goose_count += 1;
                    }
                }

                //3. does num above exist

                if ( k1 != 0){
                    top_num = true;
                    if (board[k2 + (k1-1)*x_dim] == 9){
                        goose_count += 1;
                    }
                }
                //4. does num below exist

                if ( k1 != (y_dim -1)){
                    bottom_num = true;
                    if (board[k2 + (k1 + 1)*x_dim] == 9){
                        goose_count += 1;
                    }

                }

                //check for the diagonal ones
                if ((top_num == true) && (left_num == true)){
                    if (board[k2 - 1 + (k1 - 1)*x_dim] == 9){
                        goose_count += 1;
                    }
                }

                if ((top_num == true) && (right_num == true)){
                    if (board[k2 + 1 + (k1 - 1)*x_dim] == 9){
                        goose_count += 1;
                    }
                }

                if ((bottom_num == true) && (left_num == true)){
                    if (board[k2 - 1 + (k1 + 1)*x_dim] == 9){
                        goose_count += 1;
                    }
                }

                if ((bottom_num == true) && (right_num == true)){
                    if (board[k2 +1 + (k1 + 1)*x_dim] == 9){
                        goose_count += 1;
                    }
                }

                //std::cout << "goose count: " << goose_count << std::endl;

                board[k2 + k1*x_dim] = goose_count;
            }
            


        }
    }
    //code to check fcn
    // for (std::size_t k1{0}; k1 < y_dim ; ++k1){
    //     for (std::size_t k2{0}; k2 < x_dim; ++k2){
    //         std::cout << int(board[k2 + k1*x_dim]);
    //     }
    //     std::cout << std::endl;
    // }

}

bool is_game_won(char *board, std::size_t x_dim, std::size_t y_dim){
    //if all fields that do not have a goose is revelaed 
    //--> should be 0x0# format or 0x# where # != 9
    //true if won, false if not

    //create not-goose count and not goose + revealed count
    int not_goose{0};
    int not_goose_revealed{0};


    //loop through all fields and see if its a goose or not
    for (std::size_t k1{0}; k1 < y_dim; ++k1){
        for (std::size_t k2{0}; k2 < x_dim; ++k2){

            //std::cout << "0x" << std::hex << static_cast<int>(board[k2 + k1*x_dim]) << ": ";

            if ((board[k2 + k1*x_dim] != 0x29) && (board[k2 + k1*x_dim] != 0x39) && (board[k2 + k1*x_dim] != 0x09)){
                //not goose
                not_goose += 1;
                //std::cout << "Not goose, " << "not_goose_count = "<< not_goose << ", ";

                if(((board[k2 + k1*x_dim] & hidden_mask()) == false) && ((board[k2 + k1*x_dim] & marked_mask()) == false)){
                    //not marked nor hidden therefore revealed
                    not_goose_revealed += 1;
                    //std::cout << "revealed" << "revealed count =" << not_goose_revealed;
                }
            }

            //std::cout << std::endl;
        }
    } 

    //check fcn

    //std::cout << "not goose: " << not_goose << std::endl;
    //std::cout << "not goose revealed " << not_goose_revealed << std::endl;
    //if not_goose = not_goose + revealed count return true, else return false
    if (not_goose_revealed == not_goose){
        return true;
    }else{
        return false;
    }
}

int reveal(char *board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc, std::size_t y_loc){


    if((board[x_loc + y_loc*x_dim] & hidden_mask()) && (board[x_loc + y_loc*x_dim] & marked_mask())){
        //field is marked
        return 1;

    }else if(((board[x_loc + y_loc*x_dim] & hidden_mask()) == false) && ((board[x_loc + y_loc*x_dim] & marked_mask()) == false)){
        //field is revealed
        return 2;
    }else if ((board[x_loc + y_loc*x_dim] & hidden_mask()) && ((board[x_loc + y_loc*x_dim] & marked_mask()) == false)){
        //field is hidden and unmarked and turn hidden bit into 0

        board[x_loc + y_loc*x_dim] &= ~hidden_mask();

        if (board[x_loc + y_loc*x_dim] == 0x09){
            //field is goose
            return 9;
        }else if(board[x_loc + y_loc*x_dim] == 0x00){
            //field is zero, reveal the 8 neighboring field
            //

            bool left_num{false};
            bool right_num{false};
            bool top_num{false};
            bool bottom_num{false};

            if ( x_loc != 0){
                left_num = true;

            //board[k2 - 1 + y_loc*x_dim]
                if ((board[x_loc - 1 + y_loc*x_dim] & hidden_mask()) && ((board[x_loc -1 + y_loc*x_dim] & marked_mask()) == false)){
                    //neighboring field is hidden and not 9
                    board[x_loc -1 + y_loc*x_dim] &= ~hidden_mask();

                }
            }
            //2. does num on right exist
            if ( x_loc != (x_dim -1)){
                right_num = true;
                //board[x_loc + 1 + y_loc*x_dim]

                if ((board[x_loc + 1 + y_loc*x_dim] & hidden_mask()) && ((board[x_loc + 1 + y_loc*x_dim] & marked_mask()) == false)){
                    //neighboring field is hidden and not 9
                    board[x_loc + 1 + y_loc*x_dim] &= ~hidden_mask();

                }
            }

            //3. does num above exist

            if ( y_loc != 0){
                top_num = true;
                //board[x_loc + (y_loc-1)*x_dim]
                
                if ((board[x_loc + (y_loc-1)*x_dim] & hidden_mask()) && ((board[x_loc + (y_loc-1)*x_dim] & marked_mask()) == false)){
                    //neighboring field is hidden and not 9
                    board[x_loc + (y_loc-1)*x_dim] &= ~hidden_mask();

                }
            }
            //4. does num below exist

            if ( y_loc != (y_dim -1)){
                bottom_num = true;
                
                //board[x_loc + (y_loc + 1)*x_dim]

                if ((board[x_loc + (y_loc+1)*x_dim] & hidden_mask()) && ((board[x_loc + (y_loc+1)*x_dim] & marked_mask()) == false)){
                    //neighboring field is hidden and not 9
                    board[x_loc + (y_loc+1)*x_dim] &= ~hidden_mask();

                }

            }

            if ((top_num == true) && (left_num == true)){
                //board[x_loc - 1 + (y_loc - 1)*x_dim]

                if ((board[x_loc - 1 + (y_loc - 1)*x_dim] & hidden_mask()) && ((board[x_loc - 1 + (y_loc - 1)*x_dim] & marked_mask()) == false)){
                    //neighboring field is hidden and not 9
                    board[x_loc - 1 + (y_loc - 1)*x_dim] &= ~hidden_mask();

                }
            
            }

            if ((top_num == true) && (right_num == true)){
                //board[x_loc + 1 + (k1 - 1)*x_dim]
                    if ((board[x_loc + 1 + (y_loc - 1)*x_dim]& hidden_mask()) && ((board[x_loc + 1 + (y_loc - 1)*x_dim] & marked_mask()) == false)){
                    //neighboring field is hidden and not 9
                    board[x_loc + 1 + (y_loc - 1)*x_dim] &= ~hidden_mask();

                }
                
            }

            if ((bottom_num == true) && (left_num == true)){
                //board[x_loc - 1 + (k1 + 1)*x_dim]
                    if ((board[x_loc - 1 + (y_loc + 1)*x_dim] & hidden_mask()) && ((board[x_loc - 1 + (y_loc + 1)*x_dim] & marked_mask()) == false)){
                    //neighboring field is hidden and not 9
                    board[x_loc - 1 + (y_loc + 1)*x_dim] &= ~hidden_mask();

                }
                
            }

            if ((bottom_num == true) && (right_num == true)){
                //board[x_loc +1 + (y_loc + 1)*x_dim]
                    if ((board[x_loc +1 + (y_loc + 1)*x_dim] & hidden_mask()) && ((board[x_loc +1 + (y_loc + 1)*x_dim] & marked_mask()) == false)){
                    //neighboring field is hidden and not 9
                    board[x_loc +1 + (y_loc + 1)*x_dim] &= ~hidden_mask();

                }             
            }
        }
        return 0;               
            

    }
}

//helper fnc
//void change_if_hidden()
