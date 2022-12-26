#include "Team.h"

Team::Team(int id, Team* team): m_tTeamID(id),
                                m_games(0),
                                m_fatherGames(0),
                                m_size(0),
                                m_empty(true),
                                m_fatherIsBuyer(true),
                                m_currentTeam(team),
                                m_father(nullptr),
                                m_totalPerm(permutation_t::neutral()),
                                m_relativePerm(permutation_t::neutral()){
}

const permutation_t &Team::teamValue() const {
    return findAUX(this)->m_totalPerm;
}

int Team::getSize() const {
    return m_size;
}

void Team::insertPlayer(Player& player) {
    m_empty = false;
    m_totalPerm = m_totalPerm * player.getSpirit();
    player.setPartSpirit(m_totalPerm);
    m_tAbility += player.getAbility();
    player.setTeam(this);
}

Team* Team::purchase(Team* toPurchase) {
    if(m_empty){
        toPurchase->m_tTeamID = m_tTeamID;
        return toPurchase;
    }
    if(toPurchase->m_empty){
        return this;
    }
    Team* father;
    Team* son;
    if(m_size >= toPurchase->m_size){
        father = this;
        son = toPurchase;
        toPurchase->m_fatherIsBuyer = true;
    }else{
        father = toPurchase;
        son = this;
        m_notBuyerFatherKey = father->m_relativePerm;
        m_fatherIsBuyer = false;
    }
    toPurchase->m_relativePerm = m_totalPerm * toPurchase->m_relativePerm;
    father->m_size++;
    father->m_totalPerm = m_totalPerm * toPurchase->m_totalPerm;
    father->m_tTeamID = this->m_tTeamID;
    father->m_tAbility += son->m_tAbility;
    father->m_tPoints += son->m_tPoints;
    son->m_fatherGames = father->m_games;
    return father;
}


int Team::getRelGames() const {
    int gameSum = m_games;
    const Team* ptr = this;
    while(ptr->m_father){
        gameSum+= ptr->m_father->m_games - ptr->m_fatherGames;
        ptr = ptr -> m_father;
    }
    return gameSum;
}

permutation_t Team::getRelativePerm() const {
    permutation_t res = permutation_t::neutral();
    permutation_t addition = permutation_t::neutral();
    const Team* ptr = this;
    const Team* father = ptr->m_father;

    while(father){
        if(ptr->m_fatherIsBuyer){
            addition = ptr->m_relativePerm;
        }else{
            addition = father->m_relativePerm * m_notBuyerFatherKey.inv() * m_totalPerm.inv();
            if(m_father->m_fatherIsBuyer){
                ptr = father;
            }
        }
        res = addition * res;
        ptr = ptr->m_father;
        if(!ptr)
            break;
        father = ptr->m_father;
    }
    return res;
}

int Team::getId(){
    return findAUX(this)->m_tTeamID;
}

int Team::getId() const{
    return findAUX(this)->m_tTeamID;
}

Team* Team::findAUX(Team* node){
    if(!node->m_father) {
        return node;
    }
    Team* root = findAUX(node->m_father);
    node->m_games = getRelGames();
    node->m_relativePerm = getRelativePerm();
    if(node->m_father != root){
        node->m_fatherIsBuyer = node->m_father->m_fatherIsBuyer;
    }
    node->m_father = root;
    return root;
}

const Team * Team::findAUX(const Team *node) {
    if(!node->m_father) {
        return node;
    }
    while(node->m_father)
        node = node->m_father;
    return node;
}

permutation_t Team::getBasePerm() {
    findAUX(this);
    return m_relativePerm;
}

int Team::getGames() {
    findAUX(this);
    return m_games;
}
int Team::getGames() const{
    return getRelGames();
}

int Team::getPoints() {
    return findAUX(this)->m_tPoints;
}

int Team::getPoints() const {
    return findAUX(this)->m_tPoints;
}

int Team::getAbility() {
    return findAUX(this)->m_tAbility;
}
int Team::getAbility() const{
    return findAUX(this)->m_tAbility;
}
