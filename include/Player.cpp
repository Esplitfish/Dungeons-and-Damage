#include <iostream>
#include <vector>
#include <random>
#include <array>

#include "classes.hpp"

using namespace std;

// Oh boy I love this new thing that I already forgot the name of
// Me several weeks later: instantiation
// Hope I spelled that right
Player::Player(array<int32_t, 8> s, array<int16_t, 5> i, vector<string> o, vector<string> l, string p, string a, string c, string n):
stats(s), items(i), options(o), spells(l), passive(p), activated(a), classname(c), playername(n),
current_hp(s[MAX_HP]), current_mp(s[MAX_MP]), current_itus(s[ITEM_USES])
{}

// Subtract health from the player and change dead to match being over 0HP
void Player::damage(int32_t amount, string type){

    // Knights take 2 less damage, with a minimum of zero
    if (passive == KNIGHT_PASSIVE){
        if (amount < 0) amount--; // ?
        else {
            amount -= 2;
            if (amount < 0) amount = 0;
        }
    // Rouges take half damage from magic attacks if they can dodge it
    } else if (passive == ROUGE_PASSIVE){
        if (type == "magic" && amount <= (stats[SPEED] + stats[ATTACK_BONUS])){
            amount = round(amount / 2);
        }
    }
    current_hp -= amount;
    roll(playername + " took " + to_string(amount) + " points of damage!");
    dead = (current_hp <= 0);
}

// Add health to the player and don't let it go over its max HP
void Player::heal(int32_t amount){
    if (passive == BARBARIAN_PASSIVE) amount = ceil(amount * 1.5);
    current_hp += amount;
    if (current_hp > stats[MAX_HP]) current_hp = stats[MAX_HP];
}

// Advance to the next turn
void Player::next_turn(){
    string selected_option;

    // Barbarians heal for 3 total damage (after 1.5x amplifier)
    if (passive == BARBARIAN_PASSIVE) heal(2);
    
    // Advance advantage/disadvantage
    if (effects.adtr > 0){
        effects.adtr--;
        if (effects.adtr == 0) effects.adv = 0;
    }

    // MP regain after each turn
    current_mp += stats[MP_REFRESH];
    // Cap to maximum MP
    if (current_mp > stats[MAX_MP]) current_mp = stats[MAX_MP];
    
    if (passive == BARD_PASSIVE){

        // If you are a Bard, select your next set of stats
        array<int32_t, 8> stat_block;
        while (true){
            options = {
                "Keep current stats",
                "Offensive",
                "Defensive",
                "Subsidiary",
                "Magical",
                "Well rounded (default)",
                "See current stats",
                "See stats of each"
            };
            selected_option = roll_list(options, "What set of stats would you like to change to? ", true, false, (bool*)true);
            if (selected_option == "Keep current stats") break;
            else if (selected_option == "Offensive") stat_block = BARD_OFFENSIVE_STATS;
            else if (selected_option == "Defensive") stat_block = BARD_DEFENSIVE_STATS;
            else if (selected_option == "Subsidary") stat_block = BARD_SUBSIDIARY_STATS;
            else if (selected_option == "Magical") stat_block = BARD_MAGICAL_STATS;
            else if (selected_option == "Well-rounded") stat_block = BARD_DEFAULT_STATS;

            // I structured it like this to differentiate between changing stats and seeing info
            else {
                if (selected_option == "See current stats"){
                    roll("HP: " + to_string(current_hp) + "/" + to_string(stats[MAX_HP])); wait();
                    roll("Attack Damage: " + to_string(stats[ATTACK])); wait();
                    roll("Attack (To Hit) Bonus: " + to_string(stats[ATTACK_BONUS])); wait();
                    roll("Defense: " + to_string(stats[DEFENSE])); wait();
                    roll("MP: " + to_string(current_mp) + "/" + to_string(stats[MAX_MP])); wait();
                    roll("Item Uses: " + to_string(stats[ITEM_USES])); wait();
                    roll("Speed: " + to_string(stats[SPEED])); wait();
                    confirm("Above, you will find your stats.\nPress enter to continue");
                }

                else if (selected_option == "See stats of each"){
                    array<array<int32_t, 8>, 5> array_mess = {
                        BARD_OFFENSIVE_STATS,
                        BARD_DEFENSIVE_STATS,
                        BARD_SUBSIDIARY_STATS,
                        BARD_MAGICAL_STATS,
                        BARD_DEFAULT_STATS
                    };
                    roll("HP, ATK, ATKBON, DEF, MP, ITUS, SPD"); wait();
                    for (uint32_t i; i < 5; i++){
                        switch (i){
                            case 0: roll("Offensive: ");
                            case 1: roll("Defensive: ");
                            case 2: roll("Subsidiary: ");
                            case 3: roll("Magical: ");
                            case 4: roll("Well-rounded: ");
                        }
                        for (uint32_t j; j < 8; j++){
                            roll(to_string(array_mess[i][j]));
                            if (j != 7) roll(", ");
                        }
                        roll("\n"); wait();
                    }
                }
                continue;
            }

              // Lets say you have 2 HP and try to switch to a stat block with 3 less HP than your current one
             // This would put you at -1 HP, which is not allowed
            // That's what this is for
            int32_t damage, mp_used;
            damage = stats[MAX_HP] - current_hp;
            mp_used = stats[MAX_MP] - current_mp;
            if (damage >= stat_block[MAX_HP]){
                confirm("You cannot switch to that set of stats, as that would put you at or below 0 HP.");
                continue;
            }
            stats = stat_block;
            current_hp = stats[MAX_HP] - damage;
            current_mp = stats[MAX_MP] - mp_used;
            if (current_mp < 0) current_mp = 0;
        }
    }
}

void Player::take_turn(vector<Player*> players){

      // Check if activated ability is ready
     // Will return (turns taken on cooldown / needed turns) if on cooldown
    // Will return "Ready" if ability is able to be used
    bool turn_correct = true;
    while (turn_correct){
    
    string ready = "";
    if (effects.ability_cooldown_current == effects.ability_cooldown){
        ready = " - Ready";
    } else {
        ready = " (" + to_string(effects.ability_cooldown_current) 
        + "/" + to_string(effects.ability_cooldown) + ")";
    }
    vector<string> options = {
        "Attack",
        "Item",
        "Magic",
        activated + ready,
        "Pass",
        "Run"
    };
    string option = roll_list(options);
    uint32_t I_option;

    if (option == "Attack"){
        
        // Select a player to be hit
        I_option = select_player(players);

        if (random_num(1, 20, true) + stats[ATTACK_BONUS] >= players[I_option]->stats[DEFENSE]){
            players[I_option]->damage(stats[ATTACK] + effects.dmgBON, "Basic"); // Deal damage to that player
            effects.dmgBON = 0; // Reset damage bonus
                               // Yes, it will only reset after hitting
            turn_correct = false;
            break;
        }
    } else if (option == "Item"){
        bool item_correct = true;
        while (item_correct){
        options = {
            "Back",
            "Throw Spoon - " + to_string(items[SPOON]),
            "Throw Knife - " + to_string(items[KNIVE]),
            "Drink Potion - " + to_string(items[POTION]),
            "Set up Fence - " + to_string(items[FENCE]),
        };
        if (items[DEAGLE] > 0){
            options.push_back("Shoot Deagle - " + to_string(items[DEAGLE]));
        }
        roll(playername + " has " + to_string(current_itus) + "/" + to_string(stats[ITEM_USES]) + " item uses.");
        option = roll_list(options);
        if (option == "Back"){
            item_correct = false;
            break;
        } else {
            if (option == "Throw Spoon - " + to_string(items[SPOON])){
                if (items[SPOON] > 0){
                    int64_t tetanus_user = random_num(1, 1000, false);
                    int64_t tetanus_reciever = random_num(1, 1000, false);
                    I_option = select_player(players);
                    
                } else {
                    confirm("You don't have any of those");
                    item_correct = true;
                    continue;
                }
            }
        }}
    }}
    return;
}