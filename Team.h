//
// Created by ohadr on 11/26/2022.
//

#ifndef AVL_TREES_1_H_TEAM_H
#define AVL_TREES_1_H_TEAM_H
#include "Player.h"
#include "AVL_Map.h"
#include "Hash_Map.h"
#include "Team.h"

class Team{
private:
    Team *findAUX(Team *node);
    static const Team *findAUX(const Team *node) ;
    permutation_t getRelativePerm() const;
    int getRelGames() const;
    int getSize() const;

public:
    Team::Team(int id, Team* team)
    ~Team()=default;
    Team(const Team& t) = default; ///cannot copy a team, each team unique
    Team& operator=(const Team& t)=default;

    ///-------getters-------

    const permutation_t& teamValue() const;
    int getId();
    int getId() const;
    int getGames();
    int getGames() const;
    int getPoints();
    int getPoints() const;
    int getAbility();
    int getAbility() const;

    permutation_t getBasePerm();
    ///--------setters-------

    Team* insertPlayer(const Player& playerPerm);
    Team * purchase(Team *toPurchase);

    void loseGame();
    void tieGame();
    void playGame();

private:
    int m_tTeamID;
    int m_tPoints;
    int m_tAbility;

    int m_games;
    int m_fatherGames;

    int m_size;

    bool m_empty;
    bool m_fatherIsBuyer;

    std::shared_ptr<Team> m_currentTeam;
    Team* m_father;

    permutation_t m_totalPerm;
    permutation_t m_relativePerm;
    permutation_t m_notBuyerFatherKey;
};


#endif //AVL_TREES_1_H_TEAM_H