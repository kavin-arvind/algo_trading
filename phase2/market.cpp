#include "market.h"
#include "header.h"

AVLMap EOD; //{company_name, [profit,bought,sold]}
AVLMap2 stock_b; //{stock,<heap of BUY quotes>}
AVLMap2 stock_s; //{stock,<heap of BUY quotes>}

int money_transfered = 0;
int no_of_trades = 0;
int no_of_shares_traded = 0;

market::market(int argc, char** argv)
{

}

void market::start()
{
    std::ifstream inputfromoutput("output.txt");
    std::string line;
    bool valid = true;

    while (true)
    {
        // if(!std::getline(inputfromoutput, line)) break;
        std::getline(inputfromoutput, line);
        if(line == "!@") break;

        // now line has the correct line we needed..
        Node* a = line_process_to_node(line,valid);
        
        // f.broker = b->broker;
        // f.exptime = b->exptime;
        // f.mode = b->mode;
        // f.price = b->price;
        // f.quantity = b->quantity;
        // f.stock = b->stock;
        // f.timestamp = b->timestamp;
        
        //delete b;

        //Node* a = &f;

        if(a!=nullptr){
            //std::cout<<a->broker<<""<<a->exptime<<""<<a->mode<<""<<a->price<<""<<a->quantity<<""<<a->stock<<""<<a->timestamp<<std::endl;

            std::string ouT = "";

            // NODE pointer 'a' has the node we need to process right?
            if (a->mode =='s'){
                if(stock_b.containsKey(a->stock)){ // check if it is present in BUY mode
                    Heap* b_heap = stock_b.getValue(a->stock);
                    Node* top_node = b_heap->getTop();

                    while(top_node != nullptr && a->timestamp > top_node->exptime){
                        b_heap->extractMin(); // simply delete top node
                        top_node = b_heap->getTop();
                    }

                    if(top_node != nullptr && (-1)*top_node->price >= a->price && top_node->quantity < a->quantity){
                        while(top_node != nullptr && (-1)*top_node->price >= a->price && top_node->quantity < a->quantity){

                            bool flagk=true;

                            while(top_node != nullptr && a->timestamp > top_node->exptime){
                                flagk = false;
                                b_heap->extractMin(); // simply delete top node
                                top_node = b_heap->getTop();
                            }

                            if(flagk == false){continue;}
                            
                            a->quantity -= top_node->quantity;

                            // output line processing WITHOUT NEWLINE
                            ouT = top_node->broker + " purchased " + std::to_string(top_node->quantity) + " share of " + a->stock + " from " + a->broker + " for $" + std::to_string((-1)*top_node->price) + "/share" ;
                            std::cout<<ouT<<std::endl;
                            money_transfered+= (-1)*top_node->price*top_node->quantity;
                            no_of_shares_traded+= top_node->quantity;
                            no_of_trades +=1;

                            // EOD processing
                            // for a which is SELL 
                            if(EOD.containsKey(a->broker)){
                                std::vector<int> temp1 = EOD.getValue(a->broker);
                                temp1[0] += (-1)*top_node->price*top_node->quantity;
                                temp1[2] += top_node->quantity;
                                EOD.remove(a->broker);
                                EOD.insert(a->broker, temp1);
                            }
                            else{
                                std::vector<int> temp1(3,0);
                                temp1[0] += (-1)*top_node->price*top_node->quantity;
                                temp1[2] += top_node->quantity;
                                EOD.insert(a->broker,temp1);
                            }
                            // for top_node which is BUY
                            if(EOD.containsKey(top_node->broker)){
                                std::vector<int> temp1 = EOD.getValue(top_node->broker);
                                temp1[0] += top_node->price*top_node->quantity;
                                temp1[1] += top_node->quantity;
                                EOD.remove(top_node->broker);
                                EOD.insert(top_node->broker, temp1);
                            }
                            else{
                                std::vector<int> temp1(3,0);
                                temp1[0] += top_node->price*top_node->quantity;
                                temp1[1] += top_node->quantity;
                                EOD.insert(top_node->broker,temp1);
                            }

                            // remove top node obviously
                            b_heap->extractMin();

                            top_node = b_heap->getTop();
                        } 
                    }

                    while(top_node != nullptr && a->timestamp > top_node->exptime){
                        b_heap->extractMin(); // simply delete top node
                        top_node = b_heap->getTop();
                    }

                    if(top_node != nullptr && (-1)*top_node->price >= a->price && a->quantity > 0 && top_node->quantity >= a->quantity){
                                                        
                            top_node->quantity -= a->quantity; 
                            
                            // output line processing WITHOUT NEWLINE
                            ouT = top_node->broker + " purchased " + std::to_string(a->quantity) + " share of " + a->stock + " from " + a->broker + " for $" + std::to_string((-1)*top_node->price) + "/share" ;
                            std::cout<<ouT<<std::endl;
                            money_transfered+= (-1)*top_node->price*a->quantity;
                            no_of_shares_traded+= a->quantity;
                            no_of_trades +=1;

                            // EOD processing
                            // for a which is SELL
                            if(EOD.containsKey(a->broker)){
                                std::vector<int> temp1 = EOD.getValue(a->broker);
                                temp1[0] += (-1)*top_node->price*a->quantity;
                                temp1[2] += a->quantity;
                                EOD.remove(a->broker);
                                EOD.insert(a->broker, temp1);
                            }
                            else{
                                std::vector<int> temp1(3,0);
                                temp1[0] += (-1)*top_node->price*a->quantity;
                                temp1[2] += a->quantity;
                                EOD.insert(a->broker,temp1);
                            }
                            // for top_node which is BUY
                            if(EOD.containsKey(top_node->broker)){
                                std::vector<int> temp1 = EOD.getValue(top_node->broker);
                                temp1[0] += top_node->price*a->quantity;
                                temp1[1] += a->quantity;
                                EOD.remove(top_node->broker);
                                EOD.insert(top_node->broker, temp1);
                            }
                            else{
                                std::vector<int> temp1(3,0);
                                temp1[0] += top_node->price*a->quantity;
                                temp1[1] += a->quantity;
                                EOD.insert(top_node->broker,temp1);
                            }

                            if(top_node->quantity == 0){
                                b_heap->extractMin(); // simply delete top node
                            }
                        }


                    else{
                        if((stock_s.containsKey(a->stock))){// check if it is present in SELL mode
                            Heap* s_heap = stock_s.getValue(a->stock);
                            s_heap->insert(a);
                        }
                        else{
                            // Heap emptyheap;
                            // Heap* s_heap = &emptyheap;
                            Heap* s_heap = new Heap;
                            s_heap->insert(a);
                            stock_s.insert(a->stock, s_heap);
                        }
                    }

                }
                else{
                    if((stock_s.containsKey(a->stock))){// check if it is present in SELL mode
                        Heap* s_heap = stock_s.getValue(a->stock);
                        s_heap->insert(a);
                    }
                    else{
                        // Heap emptyheap;
                        // Heap* s_heap = &emptyheap;
                        Heap* s_heap = new Heap;
                        s_heap->insert(a);
                        stock_s.insert(a->stock, s_heap);
                    }
                }
            }

            else if (a->mode=='b'){
                if(stock_s.containsKey(a->stock)){ // check if it is present in SELL mode
                    Heap* s_heap = stock_s.getValue(a->stock);
                    Node* top_node = s_heap->peek();

                    while(top_node != nullptr && a->timestamp > top_node->exptime){
                        //std::cerr<<"exp "<<a->stock<< std::endl;
                        s_heap->extractMin(); // simply delete top node
                        top_node = s_heap->getTop();
                    }
                    //std::cerr<< a->stock<< std::endl;


                    if(top_node != nullptr && (-1)*a->price >= top_node->price && top_node->quantity < a->quantity){
                        while(top_node != nullptr && (-1)*a->price >= top_node->price && top_node->quantity < a->quantity){
                            bool flagn=true;
                            while(top_node != nullptr && a->timestamp > top_node->exptime){
                                flagn = false;
                                s_heap->extractMin(); // simply delete top node
                                top_node = s_heap->getTop();
                            }

                            if(flagn == false){continue;}
                            
                            a->quantity -= top_node->quantity;

                            // output line processing WITHOUT NEWLINE
                            ouT = a->broker + " purchased " + std::to_string(top_node->quantity) + " share of " + a->stock + " from " + top_node->broker + " for $" + std::to_string(top_node->price) + "/share" ;
                            std::cout<<ouT<<std::endl;
                            money_transfered+= top_node->price*top_node->quantity;
                            no_of_shares_traded+= top_node->quantity;
                            no_of_trades +=1;

                            // EOD processing
                            // for a which is BUY 
                            if(EOD.containsKey(a->broker)){
                                std::vector<int> temp1 = EOD.getValue(a->broker);
                                temp1[0] += (-1)*top_node->price*top_node->quantity;
                                temp1[1] += top_node->quantity;
                                EOD.remove(a->broker);
                                EOD.insert(a->broker, temp1);
                            }
                            else{
                                std::vector<int> temp1(3,0);
                                temp1[0] += (-1)*top_node->price*top_node->quantity;
                                temp1[1] += top_node->quantity;
                                EOD.insert(a->broker,temp1);
                            }
                            // for top_node which is SELL
                            if(EOD.containsKey(top_node->broker)){
                                std::vector<int> temp1 = EOD.getValue(top_node->broker);
                                temp1[0] += top_node->price*top_node->quantity;
                                temp1[2] += top_node->quantity;
                                EOD.remove(top_node->broker);
                                EOD.insert(top_node->broker, temp1);
                            }
                            else{
                                std::vector<int> temp1(3,0);
                                temp1[0] += top_node->price*top_node->quantity;
                                temp1[2] += top_node->quantity;
                                EOD.insert(top_node->broker,temp1);
                            }

                            // remove top node obviously
                            s_heap->extractMin();

                            top_node = s_heap->getTop();
                        } 
                    }

                    while(top_node != nullptr && a->timestamp > top_node->exptime){
                        s_heap->extractMin(); // simply delete top node
                        top_node = s_heap->getTop();
                    }

                    if(top_node != nullptr && (-1)*a->price >= top_node->price && a->quantity > 0 && top_node->quantity >= a->quantity){
                            
                            top_node->quantity -= a->quantity; 
                            
                            // output line processing WITHOUT NEWLINE
                            ouT = a->broker + " purchased " + std::to_string(a->quantity) + " share of " + a->stock + " from " + top_node->broker + " for $" + std::to_string(top_node->price) + "/share" ;
                            std::cout<<ouT<<std::endl;
                            money_transfered+= top_node->price*a->quantity;
                            no_of_shares_traded+= a->quantity;
                            no_of_trades +=1;


                            // EOD processing
                            // for a which is BUY
                            if(EOD.containsKey(a->broker)){
                                std::vector<int> temp1 = EOD.getValue(a->broker);
                                temp1[0] += (-1)*top_node->price*a->quantity;
                                temp1[1] += a->quantity;
                                EOD.remove(a->broker);
                                EOD.insert(a->broker, temp1);
                            }
                            else{
                                std::vector<int> temp1(3,0);
                                temp1[0] += (-1)*top_node->price*a->quantity;
                                temp1[1] += a->quantity;
                                EOD.insert(a->broker,temp1);
                            }
                            // for top_node which is SELL
                            if(EOD.containsKey(top_node->broker)){
                                std::vector<int> temp1 = EOD.getValue(top_node->broker);
                                temp1[0] += top_node->price*a->quantity;
                                temp1[2] += a->quantity;
                                EOD.remove(top_node->broker);
                                EOD.insert(top_node->broker, temp1);
                            }
                            else{
                                std::vector<int> temp1(3,0);
                                temp1[0] += top_node->price*a->quantity;
                                temp1[2] += a->quantity;
                                EOD.insert(top_node->broker,temp1);
                            }

                            if(top_node->quantity == 0){
                                s_heap->extractMin(); // simply delete top node
                            }
                        }


                    else{
                        if((stock_b.containsKey(a->stock))){// check if it is present in BUY mode
                            Heap* b_heap = stock_b.getValue(a->stock);
                            b_heap->insert(a);
                        }
                        else{
                            // Heap emptyheap;
                            // Heap* b_heap = &emptyheap;
                            Heap* b_heap = new Heap;
                            b_heap->insert(a);
                            stock_b.insert(a->stock, b_heap);
                        }
                    }

                }
                else{
                    if((stock_b.containsKey(a->stock))){// check if it is present in BUY mode
                        Heap* b_heap = stock_b.getValue(a->stock);
                        b_heap->insert(a);
           
                    }
                    else{
                        // Heap emptyheap;
                        // Heap* b_heap = &emptyheap;
                        Heap* b_heap = new Heap;
                        b_heap->insert(a);
                        stock_b.insert(a->stock, b_heap);
                    }
                }
            }

        }
                //delete a;
    }
    printing_end_stuffs(money_transfered , no_of_trades , no_of_shares_traded);
    EOD.inorderTraversal();
    
}