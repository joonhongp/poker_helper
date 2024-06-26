#include "Player.h"

/*---------------Util Functions---------------------------------*/

/*
 *  convertNumbers() changes the character number to integer number.
 *  'A', which is 1 in general, is considered 14 for convenience.
 */

int convertNumbers(char rank) {
    if (rank >= '2' && rank <= '9') {
        return rank - '0';  // Converts '2'-'9' to their respective integers
    }
    switch (rank) {
        case 'T': return 10;
        case 'J': return 11;
        case 'Q': return 12;
        case 'K': return 13;
        case 'A': return 14;
        default:
            throw std::invalid_argument("Invalid character for rank");
    }
} /* convertNumbers() */

/*
 *  convertHandToNumbers() converts a vector of card strings to
 *  a vector of integer card values.
 */

std::vector<int> convertHandToNumbers(const std::vector<std::string>& hand) {
    std::vector<int> numbers;
    for (const std::string& card : hand) {
        if (card.size() != 2) {
            throw std::invalid_argument("wrong hand format");
        }
        int number = convertNumbers(card[0]);
        numbers.push_back(number);
    }
    return numbers;
} /* convertHandToNumbers() */

/*
 *  convertHandToSuits() converts a vector of card strings to
 *  a vector of character suit values.
 */

std::vector<char> convertHandToSuits(const std::vector<std::string>& hand) {
    std::vector<char> suits;
    std::unordered_set<char> suitSet = {'C', 'S', 'D', 'H'};
    for (const std::string& card : hand) {
        if (card.size() != 2) {
            throw std::invalid_argument("wrong hand format");
        }
        char suit = card[1];
        if (suitSet.find(suit) != suitSet.end()) {
            suits.push_back(suit);
        }
        else {
            throw std::invalid_argument("wrong suit");
        }
    }
    return suits;
} /* convertHandToSuits() */

/*
 *  handsToString() will return string value of the enum.
 *  just like toString() in Java.
 */

std::string handsToString(Hands hand) {
    switch(hand) {
        case Hands::NoMatch: return "NoMatch";
        case Hands::OnePair: return "OnePair";
        case Hands::TwoPair: return "TwoPair";
        case Hands::ThreeOfAKind: return "ThreeOfAKind";
        case Hands::Straight: return "Straight";
        case Hands::Flush: return "Flush";
        case Hands::FullHouse: return "FullHouse";
        case Hands::FourOfAKind: return "FourOfAKind";
        case Hands::StraightFlush: return "StraightFlush";
        case Hands::RoyalFlush: return "RoyalFlush";
        default:
            throw std::invalid_argument("wrong hands input");
    }
} /* handsToString() */

/*
 *  makeCompleteHand() adds community cards and hole Cards.
 *  It throws an error when 
 */

std::vector<std::string> Player::makeCompleteHand(const std::vector<std::string>& communityCards) {
    std::vector<std::string> completeHand;
    std::set<std::string> cardSet;

    // Insert community cards
    for (const auto& card : communityCards) {
        if (!cardSet.insert(card).second) {
            throw std::invalid_argument("Duplicate card found: " + card);
        }
        completeHand.push_back(card);
    }

    // Insert hole cards
    for (const auto& card : holeCards) {
        if (!cardSet.insert(card).second) {
            throw std::invalid_argument("Duplicate card found: " + card);
        }
        completeHand.push_back(card);
    }

    return completeHand;
} /* makeCompleteHand() */

/*---------------Hand Detection Boolean Functions---------------------------*/

/*
 *  isOnePair() checks whether the hand has one pair or not.
 */   

bool Player::isOnePair(const std::vector<std::string>& completeHand) {
    std::vector<int> numbers = convertHandToNumbers(completeHand);
    
    std::map<int, int> counts;
    for (int number : numbers) {
        counts[number]++;
    }
    for (const auto& count : counts) {
        if (count.second == 2) {
            return true;
        }
    }
    return false;
} /* isOnePair() */

/*
 *  isTwoPair() checks whether the hand has two pairs or not.
 */

bool Player::isTwoPair(const std::vector<std::string>& completeHand) {
    std::vector<int> numbers = convertHandToNumbers(completeHand);
    std::map<int, int> counts;
    int pairCount = 0;
    for (int number : numbers) {
        counts[number]++;
    }
    for (const auto& count : counts) {
        if (count.second == 2) {
            pairCount++;
            if (pairCount >= 2) {
                return true;
            }
        }
    }
    return false;
} /* isTwoPair() */

/*
 *  isThreeOfAKind() checks whether the hand has triple or not.
 */

bool Player::isThreeOfAKind(const std::vector<std::string>& completeHand) {
    std::vector<int> numbers = convertHandToNumbers(completeHand);
    std::map<int, int> counts;
    for (int number : numbers) {
        counts[number]++;
    }
    for (const auto& count : counts) {
        if (count.second == 3) {
            return true;
        }
    }
    return false;
} /* isThreeOfAKind() */

/*
 *  isStraight() checks whether the hand has a straight or not.
 */
bool Player::isStraight(const std::vector<std::string>& completeHand) {
    std::vector<int> numbers = convertHandToNumbers(completeHand);
    std::sort(numbers.begin(), numbers.end());

    // Remove duplicates
    auto it = std::unique(numbers.begin(), numbers.end());
    numbers.erase(it, numbers.end());

    if (numbers.size() < 5) return false;

    // Check standard straight
    for (size_t i = 0; i <= numbers.size() - 5; ++i) {
        if (std::is_sorted(numbers.begin() + i, numbers.begin() + i + 5) &&
            numbers[i + 4] - numbers[i] == 4) {
            return true;
        }
    }

    // Check for a wheel straight: Ace - 2 - 3 - 4 - 5
    return numbers[0] == 2 && numbers[1] == 3 && numbers[2] == 4
           && numbers[3] == 5 && numbers[4] == 14;


    return false;
} /* isStraight() */

/*
 *  isFlush() checks whether the hand has flush or not.
 */

bool Player::isFlush(const std::vector<std::string>& completeHand) {
    std::vector<char> suits = convertHandToSuits(completeHand);
    std::map<char, int> counts;
    for (char suit : suits) {
        counts[suit]++;
        if (counts[suit] >= 5) {
            return true;
        }
    }
    return false;
} /* isFlush() */

/*
 *  isFullHouse() checks whether the hand is fullhouse or not.
 */

bool Player::isFullHouse(const std::vector<std::string>& completeHand) {
    std::vector<int> numbers = convertHandToNumbers(completeHand);
    std::map<int, int> counts;

    for (int number : numbers) {
        counts[number]++;
    }

    bool hasThreeOfAKind = false;
    bool hasPair = false;
    int tripleValue = 0;

    // Check for three of a kind
    for (const auto& count : counts) {
        if (count.second >= 3) {
            hasThreeOfAKind = true;
            tripleValue = count.first;
            break;
        }
    }

    // Check for a pair, ensuring it's not the same as the triple
    for (const auto& count : counts) {
        if (count.first != tripleValue && count.second >= 2) {
            hasPair = true;
            break;
        }
    }

    return hasThreeOfAKind && hasPair;
} /* isFullHouse() */

/*
 *  isFourOfAKind() checks whether the hand has quad or not.
 */

bool Player::isFourOfAKind(const std::vector<std::string>& completeHand) {
    std::vector<int> numbers = convertHandToNumbers(completeHand);
    std::map<int, int> counts;
    for (int number : numbers) {
        counts[number]++;
    }
    for (const auto& count : counts) {
        if (count.second == 4) {
            return true;
        }
    }
    return false;
} /* isFourOfAKind() */

/*
 *  isStraightFlush() checks if the hand is both straight and flush.
 */
bool Player::isStraightFlush(const std::vector<std::string>& completeHand) {
    std::vector<int> numbers = convertHandToNumbers(completeHand);
    std::vector<char> suits = convertHandToSuits(completeHand);
    std::map<char, std::vector<int>> suitToNumbers;

    // Group numbers by suits
    for (size_t i = 0; i < numbers.size(); ++i) {
        suitToNumbers[suits[i]].push_back(numbers[i]);
    }

    // Check each suit group for a straight flush
    for (const auto& pair : suitToNumbers) {
        const auto& nums = pair.second;
        if (nums.size() < 5) {
            continue;
        }

        std::vector<int> sortedNums(nums);
        std::sort(sortedNums.begin(), sortedNums.end());

        // Check for five consecutive numbers
        int consecutiveCount = 1; // Start with the first card already counted
        for (size_t i = 1; i < sortedNums.size(); ++i) {
            if (sortedNums[i] == sortedNums[i - 1] + 1) {
                consecutiveCount++;
            } else if (sortedNums[i] != sortedNums[i - 1]) {
                consecutiveCount = 1; // Reset count if cards are not consecutive
            }
            if (consecutiveCount >= 5) {
                return true; // Found a straight flush
            }
        }
    }

    return false; // No straight flush found
} /* isStraightFlush() */

/*
 *  isRoyalFlush() checks if the hand is a Royal Flush.
 */
bool Player::isRoyalFlush(const std::vector<std::string>& completeHand) {
    std::vector<int> numbers = convertHandToNumbers(completeHand);
    std::vector<char> suits = convertHandToSuits(completeHand);

    std::map<char, std::vector<int>> suitToNumbers;
    for (size_t i = 0; i < numbers.size(); ++i) {
        suitToNumbers[suits[i]].push_back(numbers[i]);
    }

    for (auto& entry : suitToNumbers) {
        std::vector<int>& nums = entry.second;
        std::sort(nums.begin(), nums.end());
        if (nums.size() >= 5) {
            if (nums[0] == 10 && nums[1] == 11 && nums[2] == 12 && nums[3] == 13 && nums[4] == 14) {
                return true;
            }
        }
    }
    return false;
} /* isRoyalFlush() */

/*---------------------post detection functions------------------------*/


/*
 *  findBestFiveCardHand() returns the best hand with 5 cards.
 */

std::vector<std::string> Player::findBestFiveCardHand(const std::vector<std::string>& completeHand) {
    std::vector<std::string> bestHand(completeHand.begin(), completeHand.begin() + 5);
    std::vector<std::string> currentHand(5);
    Hands bestRank = evaluateHand(bestHand);
    Hands currentRank;

    // Generate all combinations of 5 out of 7 cards
    std::vector<bool> select(7, false);
    std::fill(select.begin(), select.begin() + 5, true);
    do {
        int index = 0;
        for (int i = 0; i < 7; ++i) {
            if (select[i]) {
                currentHand[index++] = completeHand[i];
            }
        }
        currentRank = evaluateHand(currentHand);
        int comparisonResult = Game::compareSameHands(currentHand, bestHand);
        if (currentRank > bestRank || (currentRank == bestRank && comparisonResult == 1)) {
            bestRank = currentRank;
            bestHand = currentHand;
        }
    // prev_permutation atuomatically changes the boolean values of select
    } while (std::prev_permutation(select.begin(), select.end()));
    
    return bestHand;
} /* findBestFiveCardHand() */

/*
 *  evaluateHand() will evalulate what hand the player has. 
 *  Should be in order from highest hand to lowest hand because
 *  the higher ranked hand counts the lower rank as well if true.
 */

Hands Player::evaluateHand(const std::vector<std::string>& hand) {

    if (isRoyalFlush(hand)) return Hands::RoyalFlush;
    if (isStraightFlush(hand)) return Hands::StraightFlush;
    if (isFourOfAKind(hand)) return Hands::FourOfAKind;
    if (isFullHouse(hand)) return Hands::FullHouse;
    if (isFlush(hand)) return Hands::Flush;
    if (isStraight(hand)) return Hands::Straight;
    if (isThreeOfAKind(hand)) return Hands::ThreeOfAKind;
    if (isTwoPair(hand)) return Hands::TwoPair;
    if (isOnePair(hand)) return Hands::OnePair;
    return Hands::NoMatch;
} /* evaluateHand() */


/*-----------------player action functions----------------------------------------*/

/*
 *  betting() makes betting for the player.
 */

void Player::betting(int amount, Game& game) {
    // add to game totalCoin
    // change maxBetting if bigger than current
    // add to the player betting amount
    // need to remove the amount from the player's coin
    game.setTotalCoin(game.getTotalCoin() + amount);
    setCoinBet(coinBet + amount);
    setCoin(coin - amount);
    if (coinBet > game.getMaxBetting()) {
        game.setMaxBetting(amount);
    }
} /* betting() */

/*
 *  chooseAction() will choose what action to choose in the current round.
 */
int Player::chooseAction(Game& game) {
    // for now, I will choose randomly
    // after making statistical tool, this choice will be based on statistical result
    static std::mt19937 rng(std::random_device{}());  // Random number generator

    // 1. fold
    // 2. check
    // 3. call
    // 4. bet
    // 5. raise
    std::vector<int> actions = {1, 2, 3, 4, 5};

    // from round 2, if there is no bet, no fold
    if ((game.getRound() != 1) && (!game.getHasBet())) {
        actions.erase(actions.begin());
    }
    // if there is a bet, you cannot check
    // betting is only for the first time
    if (game.getHasBet()) {
        actions.erase(actions.begin() + 1);
        actions.erase(actions.begin() + 3);
    }
    // if not bet, no raise
    if (!game.getHasBet()) {
        actions.erase(actions.begin() + 4);
    }
    // if there is no raise above the current coinbet, no call
    if (getCoinBet() >= game.getMaxBetting()) {
        actions.erase(actions.begin() + 2);
    }

    std::uniform_int_distribution<> dist(0, actions.size() - 1);

   int randomIndex = dist(rng);

    return actions[randomIndex];
} /* chooseAction() */

/*
 *  doAction() runs different actions based on the choice.
 */
void Player::doAction(int action, Game& game) {
    int amountToCall;
    int amountToBet;
    int amountToRaise;
    int playerBet;
    switch (action) {
        case 1: // fold
            setIsFold(true);
            std::cout << getName() << ": fold!\n"; 
            break;
        
        case 2: // check
            std::cout << getName() << ": check!\n"; 
            break;
        
        case 3: // call
            amountToCall = game.getMaxBetting() - getCoinBet();
            betting(amountToCall, game);
            setDoneAction(true);
            std::cout << getName() << ": call! added: " << amountToCall << "\n"; 
            break;
        
        case 4: // bet
            // raise only the big blind for now
            amountToBet = game.getSmallBlind() * 2;
            betting(amountToBet, game);
            setDoneAction(true);
            std::cout << getName() << ": bet! added: " << amountToBet << "\n"; 
            // need to put everyone's doneAction to true and make this person doneaction true
            game.makeDoneActionFalse();
            setDoneAction(true);
            break;
        
        case 5: // raise
            // raise only the big blind for now
            amountToRaise = game.getMaxBetting() + (game.getSmallBlind() * 2);
            game.setMaxBetting(amountToRaise);
            playerBet = amountToRaise - getCoinBet();
            betting(playerBet, game);
            setDoneAction(true);
            std::cout << getName() << ": raise! added: " << (game.getSmallBlind() * 2) << "\n"; 
            // need to put everyone's doneAction to true and make this person doneaction true
            game.makeDoneActionFalse();
            setDoneAction(true);
            break;
        
        default:
            throw std::invalid_argument("wrong action number. Error thrown");
    }
} /* doAction() */
