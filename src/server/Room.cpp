#include "Room.h"

Room::Room() {
    _members = {};
}

Room::~Room() {}

//leader is located at index 0
//newest member is located at index length-1
void Room::addMember(int newMember) {
    if (_uniqueMembers.find(newMember) == _uniqueMembers.end()) {
        _members.push_back(newMember);
        _uniqueMembers.insert(newMember);
    }
}