# Dungeons & Damage
<p>Yo how do you connect a godot project to a repository 😥<p>
<p>OKAY i figured out how to connect a godot project to github it kinda sucked but i got it</p>

# Plans
- [ ] Remake the game in godot
- [ ] Get someone else to do the art for me cause i can't draw (same for music)
- [ ] I really want to try to get online working at some point (probably another separate branch)
- [ ] AI for single player
- [ ] Make the game an itch page or something if I can get it to that point

<p>⚔️⚔️⚔️⚔️⚔️⚔️⚔️⚔️⚔️⚔️⚔️⚔️</p>

## Character Stats:

### Stat Descriptions:
<p>hp: Health Points. This goes to 0, you lose the game.</p>
<p>atk: Attack Damage. This is how much damage you do on a sucessful hit.</p>
<p>atkBON (ATK(FOUND HERE)): Attack Roll Bonus. This is what is added to the random 1-20 number when determining a hit</p>
<p>def: Defence. When you are attacked, this number is compared to a random number (1-20) plus their atkBON</p>
<p>mp: Magic Points/Power: Determines how much magic you can use. Different spells cost different amounts of MP</p>
<p>mpBON (MP(FOUND HERE)): Magic Point/Power Refresh. At the end of each turn, you get this much MP back</p>
<p>itus: Item Uses. Each character can use a certain amount of items on their turn, being the value of item_uses</p>
<p>spd: Speed. Turn order is determined by speed stats. The higher speed stat player goes first. In the event of a tie, the person who goes first is chosen randomly. That will then be the turn order for the rest of the game</p>


### Knight stats:
    HP: 35  ATK: 7(+5)  DEF: 16  MP: 5(+3)  SPD: 3  ItUs: 2
    Passive: Fortitude. -2 Damage taken

### Peashooter stats:
    HP: 26  ATK: 9(+4)  DEF: 14  MP: 7(+3)  SPD: 5  ItUs: 3
    Passive: Charge. For every turn ended at maxMP, add 2 to your next attack

### Rouge stats:
    HP: 20  ATK: 10(+3)  DEF: 13  MP: 6(+2)  SPD: 4  ItUs: 4
    Passive: Accelerate. After every turn, increase your speed by 1 (max 7), and your attack roll bonus by 1 (max 9)

### Mage stats:
    HP: 21  ATK: 5(+2)  DEF: 11   MP: 5(+2)  SPD: 4  ItUs: 2
    Passive: Zoning In. After every turn, increase your maximum MP by 1 (max 10), and set your MP refresh to 1/2 of your maximum MP

### Skele stats:
    HP: 30  ATK: 7(+3)  DEF: 12  MP: 7(+5)  SPD: 6  ItUs: 4
    Passive: Focused. Cannot take more than 14 damage in a single hit (excluding instant kills)

### Bard stats:
    HP: 27  ATK: 6(+4)  DEF: 14  MP: 4(+2)  SPD: 2  ItUs: 5
    Passive: Jack of All Trades. After every turn, you may switch to a different set of stats (as long as it won't kill you)

### Barbarian stats:
    HP: 40  ATK: 12(+2)  DEF: 15  MP: 2(+1)  SPD: 1  ItUs: 1
    Passive: Healthy. Heal 3 damage after every turn, and get 1.5x to any healing

## Spells:
<p>Fireball - 5MP: Does an amount of damage between your maximum MP and 1.5x your maximum MP. Can be blocked if the magic is too weak</p>
<p>Summon Random Item - 2MP: Adds a random item to your inventory. Different items have different chances of appearing</p>
<p>Gain Advantage - 3MP: Give yourself advantage, even if you had disadvantage</p>
<p>Impose Disadvantage - 4MP: Remove the other player's advantage, or give them disadvantage</p>
<p>Heal 20% - 4MP: Heals 20% of your maximum HP</p>
<p>Damage Boost - 2MP: Add 1/3 of your ATK to your next attack, hit or miss</p>

## Items:
<p>Note - the % is the chance of appearing from the Summon Random Item spell</p>
<p>Rusty Spoon - 10%: Do 1 point of unblockable damage. Has a .1% chance to kill either player instantly from tetanus</p>
<p>Knives - 40%: Do 1-5 points of unblockable damage, with a 1/10 chance to miss</p>
<p>Healing Potion - 40%: Heal 10% of your maximum HP</p>
<p>Chain Link Fence - 10%: Block one hit, regardless of damage</p>

## Credits
<p>Coder - Splitfish</p>
<p>Coder - Emilio</p>
<p>Teacher - Mr. Young</p>
<p>Assistant Teacher - CodeHS</p>
<p>Online Resource - W3schools.com</p>
<p>Online Resource - GeeksforGeeks.org</p>
