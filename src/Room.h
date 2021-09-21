#include<vector>
#include<set>

class Room {
    public:
        Room();
        ~Room();
        void addMember(int newMember);
        int getLeader() {return _members[0];}
        int getNewest() {return _members[_members.size()-1];}
        int size() {return _members.size();}
        std::vector<int> members() {return _members;}
    private:
        //could be public, but I'd rather have one way to get this information
        std::vector<int> _members;
        std::set<int> _uniqueMembers;
};