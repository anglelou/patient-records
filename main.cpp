//
//  main.cpp
//  Hospital Record System
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <initializer_list>

#define ARG_ERROR   1
#define FILE_ERROR  2


// Resource Acquisition Is Initialization
class application
{
public:
    application(const std::string msg="done")
    : message_(msg)
    { std::cout << "STARTING APP: " << __FILE__ << "...\n"; }
    
    ~application()
    { std::cout << "\n\t\t" << message_ << ".\n"; }
    
private:
    std::string message_;
};


// Patient struct
typedef struct patient patient;

struct patient
{
    // Member Variables
    size_t id_;
    size_t height_;
    size_t weight_;
    size_t age_;
    
    // Constructor
    patient(size_t id = 0, size_t height = 0, size_t weight = 0, size_t age = 0)
    : id_(id), height_(height), weight_(weight), age_(age) {}
    
    bool operator< (const patient& other) {return id_ < other.id_;}
    bool operator==(const patient& other) {return id_ == other.id_;}
    
    friend std::istream& operator>>(std::istream& is, patient& patient)
    {
        is >> patient.id_;      is.ignore();
        is >> patient.height_;  is.ignore();
        is >> patient.weight_;  is.ignore();
        is >> patient.age_;     is.ignore();
        return is;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const patient& patient)
    {
        return os << patient.id_ << "\t\t "
                  << patient.height_ << "\t\t "
                  << std::setw(3) << patient.weight_ << "\t\t\t"
        << patient.age_;
    }
};


// Patient Node struct for linked list
typedef struct pnode pnode;

struct pnode
{
    pnode (patient()) {}
    pnode (const patient& info, pnode* prev=nullptr, pnode* next=nullptr)
    : info_(info), prev_(prev), next_(next) {}
    
    patient  info_;
    pnode* next_;
    pnode* prev_;
    
};


// Linked list class
class patient_linklist
{
public:
    patient_linklist()
    : head_(nullptr), tail_(nullptr), size_(0) {}
    
    patient_linklist(const std::initializer_list<patient>& li)
    : patient_linklist()
    {
        for (auto v : li) { prepend (new pnode(v, nullptr, head_));}
    }
    
    void prepend(pnode* node)
    {
        if(size_ == 0)
        {
            node->prev_ = node->next_ = nullptr;
            tail_ = head_ = node;
        }
        else
        {
            node->prev_ = nullptr;
            node->next_ = head_;
            head_->prev_=node;
            head_ =node;
        }
        ++size_;
    }
    
    void append(pnode* node)
    {
        if(size_ == 0) {prepend(node); return; }
        pnode* prev = tail_;
        prev->next_ = node;
        node->prev_ = prev;
        node->next_ = nullptr;
        tail_ = node;
        ++size_;
    }
    
    // Insert node in front of node at index
    void insert(pnode* node, size_t index)
    {
        checkindex(index);
        if (index == 0) {prepend(node); return; }
        if (index == size_) {append(node); return; }
        pnode* p = head_;
        pnode* prev = p;
        while (index > 0)
        {
            prev = p;
            p = p->next_;
            --index;
        }
        prev->next_ = node;
        node->prev_ = prev;
        node->next_ = p;
        p->prev_ = node;
        ++size_;
    }
    
    // Insert nodes in ascending order
    void insert(pnode* node)
    {
        pnode* p = head_;
        if(size_ == 0 || node->info_ < p->info_)
        {
            prepend(node);
            return;
        }
        else if (size_ == 1 || tail_->info_ < node->info_)
        {
            append(node);
            return;
        }
        else
        {
            pnode* p = head_;
            pnode* prev = p;
            while ( p->info_ < node->info_ && p->next_ != nullptr)
            {
                prev = p;
                p = p->next_;
            }
            prev->next_ = node;
            node->prev_ = prev;
            node->next_ = p;
            p->prev_ = node;
            ++size_;
        }
    }
    
    void delete_at(size_t index)
    {
        checkindex(index);
        if (index == 0)
        {
            pnode* p = head_;
            head_ = head_->next_;
            delete p;
        }
        else if (size_ == 1)
        {
            delete head_;
            head_ = tail_ = nullptr;
        }
        else if (size_ == 2)
        {
            if (index == 1)
            {
                delete tail_;
                head_->next_ = nullptr;
                tail_ = head_;
            }
            else
            {
                delete head_;
                tail_->prev_ = nullptr;
                head_ = tail_;
            }
        }
        else if (index == size_ - 1)
        {
            pnode* p = tail_;
            tail_ = tail_->prev_;
            tail_->next_ = nullptr;
            delete p;
        }
        else
        {
            pnode* p = head_;
            pnode* prev = p;
            pnode* next = p->next_;
            while(index > 0 && next->next_ != nullptr)
            {
                prev = p;
                p = p->next_;
                next = next->next_;
                --index;
            }
            prev->next_ = next;
            next->prev_ = prev;
            delete p;
        }
        --size_;
    }
    
    size_t sum_weights(pnode* p)
    {
        if ( p == nullptr) {return 0;}
        
        return p->info_.weight_ + sum_weights(p->next_);
    }
    
    size_t size() const {return size_;}
    bool empty() const {return size_ == 0;}
    pnode* front() const {return head_;}
    pnode* back() const {return tail_;}
    
    friend std::ostream& operator<< (std::ostream& os, const patient_linklist& li)
    {
        if (li.empty()) {return os << "Patient List is empty";}
        pnode* p = li.head_;
        os << "\n";
        os << "\n // ========================================== \n";
        os << "PATIENT   HEIGHT(cm)   WEIGHT(kg)    AGE(yrs)\n";
        os << "===============================================\n";
        
        while (p != nullptr)
        {
            os << " " << p->info_ << "\n";
            p = p->next_;
        }
        return os << "\n";
    }
    
private:
    void checkindex (size_t index)
    {
        if (index >= size_) {throw new std::invalid_argument("index out of range");}
    }
    pnode* head_;
    pnode* tail_;
    size_t size_;
};


void least_most (const patient_linklist& li,
                 patient_linklist& youngest, patient_linklist& oldest,
                 patient_linklist& shortest, patient_linklist& tallest,
                 patient_linklist& lightest, patient_linklist& heaviest)
{
    if (li.empty()) {throw new std::invalid_argument("cannot find min/max of list");}
    pnode* p = li.front();
    patient bio = p->info_;
    size_t ht = bio.height_;
    size_t wt = bio.weight_;
    size_t age = bio.age_;
    
    size_t minht = ht;
    size_t maxht = ht;
    size_t minwt = wt;
    size_t maxwt = wt;
    size_t minage = age;
    size_t maxage = age;
    
    while (p != nullptr)
    {
        patient bio = p->info_;
        ht = bio.height_;
        wt = bio.weight_;
        age = bio.age_;
        
        if (ht > maxht)     {maxht =ht;}
        if (wt > maxwt)     {maxwt = wt;}
        if (age > maxage)   {maxage = age;}
        
        if (ht < minht)     {minht = ht;}
        if (wt < minwt)     {minwt = ht;}
        if (age < minage)   {minage = age;}
        
        p = p->next_;
    }
    
    p = li.front();
    while (p != nullptr)
    {
        patient bio = p->info_;
        size_t ht = bio.height_;
        size_t wt = bio.weight_;
        size_t age = bio.age_;
        
        if (ht == maxht)    {tallest.prepend(new pnode(bio));}
        if (wt == maxwt)    {heaviest.prepend(new pnode(bio));}
        if (age == maxage)  {youngest.prepend(new pnode(bio));}
        
        if (ht == minht)    {shortest.prepend(new pnode(bio));}
        if (wt == minwt)    {lightest.prepend(new pnode(bio));}
        if (age == minage)  {youngest.prepend(new pnode(bio));}
        
        p = p->next_;
    }
}


int main(int argc, const char * argv[]) {
    application care;
    
    if (argc != 2)
    {
        std::cerr << "\nUsage: ./prob <inputfile>\n";
        exit(ARG_ERROR);
    }
    
    std::ifstream ifs;
    std::string inputfile = argv[1];
    ifs.open(inputfile);
    if(!ifs.is_open())
    {
        std::cerr << "CANNOT OPEN INPUT FILE: " << inputfile << "\n";
        exit(ARG_ERROR);
    }
    
    patient bio;
    patient_linklist li;
    
    // Ascending order insertion
    while (ifs >> bio) { li.insert(new pnode(bio)); }
    ifs.close();
    std::cout << "\n" << li.size() << " records read from file: " << inputfile << "\n";
    std::cout << li << "\n";
    
    patient_linklist youngest, oldest, shortest, tallest, lightest, heaviest;
    least_most(li, youngest, oldest, shortest, tallest, lightest, heaviest);
    
    std::cout << "youngest patient(s) is(are): " << youngest;
    std::cout << "oldest patient(s) is(are): " << oldest << "\n";
    
    std::cout << "shortest patients is(are): " << shortest;
    std::cout << "tallest patients is(are): "    << tallest << "\n";
    
    std::cout << "lightest patients is(are): " << lightest;
    std::cout << "heaviest patients is(are): "   << heaviest << "\n";
    
    size_t sum_weights = li.sum_weights(li.front());
    std::cout << "average weight: " << sum_weights / li.size() << " kg\n";
    
    return 0;
}
