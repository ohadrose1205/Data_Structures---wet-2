#include "worldcup23a2.h"

world_cup_t::world_cup_t()
{
	// TODO: Your code goes here
}

world_cup_t::~world_cup_t()
{
	// TODO: Your code goes here
}

StatusType world_cup_t::add_team(int teamId)
{
    if(teamId <= 0)
        return StatusType::INVALID_INPUT;
    if(m_teamsTree.find(teamId))
        return StatusType::FAILURE;
    try{
        Team* nTeam = new Team(teamId);
        StatusType checker = m_teamsTree.insert(nTeam.getShell(), teamId);
        if(checker!= StatusType::SUCCESS)
            return checker;
        return m_teamsTreeAbility.insert(nTeam.getShell(), *nTeam.getShell());
    }catch(...){
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType world_cup_t::remove_team(int teamId)
{
    if(teamId <= 0)
        return StatusType::INVALID_INPUT;
    Pair<Team_Shell*,int>* teamShell = m_teamsTree.find(teamId);
    if(!teamShell){
        return StatusType::FAILURE;
    }
    m_teamsTreeAbility.remove(*teamShell.data());
    m_outOfTTeams = m_outOfTTeams.purchase(teamShell.data());
    m_teamsTree.remove(teamShell.key());
    return StatusType::SUCCESS;
}

StatusType world_cup_t::add_player(int playerId, int teamId,
                                   const permutation_t &spirit, int gamesPlayed,
                                   int ability, int cards, bool goalKeeper)
{
    if(playerId <= 0 || teamId <=0 || !spirit.isvalid() || gamesPlayed < 0 || cards < 0)
        return StatusType::INVALID_INPUT;
    Pair<Team_Shell*,int>* teamShell = m_teamsTree.find(teamId)
    if(m_playerTable.find(playerId) || !teamShell)
        return StatusType::FAILURE;
    m_teamsTreeAbility.remove(*teamShell.data());
    StatusType checker = teamShell.data()->insertPlayer();
    if(checker != StatusType::SUCCESS)
        return checker;
    return m_teamsTreeAbility.insert(teamShell->data(),*teamShell.data());
}

output_t<int> world_cup_t::play_match(int teamId1, int teamId2)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::num_played_games_for_player(int playerId)
{

	return 22;
}

StatusType world_cup_t::add_player_cards(int playerId, int cards)
{
    if(playerId <= 0 || cards < 0)
        return StatusType::INVALID_INPUT;
    Pair<Player,int>* player = m_playerTable.find(playerId);
    Team* team = player.data().getTeam();
    if(!player || !team.active())
        return StatusType::FAILURE;
    player.data().addCards(cards);
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_player_cards(int playerId)
{
    if(playerId <= 0)
        return StatusType::INVALID_INPUT;
    Pair<Player,int>* player = m_playerTable.find(playerId);
    if(!player)
        return StatusType::FAILURE;
    return player.data().getCards();
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    if(teamId <= 0)
        return StatusType::INVALID_INPUT;
    Pair<Team_Shell*,int>* team = m_teamsTree.find(teamId);
    if(!team)
        return StatusType::FAILURE;
    return team.data().getPoints();
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i)
{
    Pair<Team_Shell*,Team_Shell>* team = m_teamsTreeAbility.select(i+1);
    if(!team)
        return StatusType::FAILURE;
    return team->data()->getId();
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId)
{
    if(playerId <= 0)
        return StatusType::INVALID_INPUT;
    Pair<Player,int>* player = m_playerTable.find(playerId);
    Team* team = player.data().getTeam();
    if(!player || !team.active())
        return StatusType::FAILURE;
    return player.data().getPartialSpirit();
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2)
{
    if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2)
        return StatusType::INVALID_INPUT;
    Pair<Team_Shell*,int>* team1 = m_teamsTree.find(teamId1);
    Pair<Team_Shell*,int>* team2 = m_teamsTree.find(teamId2);
    if(!team1 || !team2)
        return StatusType::FAILURE;
    m_teamsTree.remove(teamId2);
    m_teamsTreeAbility.remove(*team1->data());
    m_teamsTreeAbility.remove(*team2->data());
    team1->data() = team1->data().purchase(team2->data());
    return m_teamsTreeAbility.inseert(team1->data(),*team1->data());
}
