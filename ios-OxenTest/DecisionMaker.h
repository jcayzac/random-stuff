#ifndef DECISION_MAKER_INCLUDED
#define DECISION_MAKER_INCLUDED

#include <list>

class DecisionMaker {
public:
	/** @brief Base class for representating the universe logically.
	 */
	class GameState {
	public:
		GameState() { }
		GameState(const GameState&) { }
		virtual ~GameState() { }
	};
		
	/** @brief A collection of game states.
	 */
	typedef std::list<GameState*> GameStatesCollection;
	
	/** @brief Game model interface.
	 *
	 * The two methods in this interface provide the AI player will all he needs to know
	 * about your game's rules.
	 */
	class IDecisionModel {
	public:
		virtual void GetChoiceCollection(const GameState* state, GameStatesCollection& result) = 0;
		virtual int  EvaluateState(const GameState* state) = 0;
	};
	
private:
	IDecisionModel* model;
	int maximum_depth;
public:
	/** @brief Build a new AI player.
	 * @param theModel Model of the game.
	 * @param thinkAheadTurns Maximum depth of the Minimax tree.
	 */
	DecisionMaker(IDecisionModel* theModel, int thinkAheadTurns=5): model(theModel) {
		maximum_depth = thinkAheadTurns;
	}
	
	/** @brief Applies a Minimax algorithm to choose the best strategy.
	 * @param initialState Current state of the problem
	 * @param nextState Contains the new state on exit
	 * @return Expected value of nextState
	 */
	inline GameState* Decide(const GameState* initialState, int& outcome) {
		GameStatesCollection states;
		GetChoiceCollection(initialState, maximum_depth, states);
		
		// Get the child with the maximum outcome
		GameState* nextState = states.front();
		states.pop_front();
		outcome = Min(nextState, maximum_depth);
		while (!states.empty()) {
			GameState* state = states.front();
			states.pop_front();
			
			int v = Min(state, maximum_depth);
			if (v>outcome) {
				outcome=v;
				delete nextState;
				nextState=state;
			}
			else delete state;
		}
		return nextState;
	}
private:
	void GetChoiceCollection(const GameState* initialState, int currentDepth, GameStatesCollection& result) {
		if (currentDepth>0) {
			try {
				model->GetChoiceCollection(initialState, result);
			}
			catch(...) {
				while(!result.empty()) {
					GameState* state = result.front();
					if (state) delete state;
					result.pop_front();
				}
				throw;
			}
		}
	}
	int Min(const GameState* initialState, int depth) {
		GameStatesCollection states;
		GetChoiceCollection(initialState, depth, states);
		
		if (states.empty())
			return model->EvaluateState(initialState);
		
		--depth;
		
		// Minimize the maximum outcome
		GameState* state = states.front();
		states.pop_front();
		int best = Max(state, depth);
		delete state;
		
		while(!states.empty()) {
			state = states.front();
			states.pop_front();
			int v = Max(state, depth);
			delete state;
			if (v < best) best = v;
		}
		return best;
	}
	int Max (const GameState* initialState, int depth) {
		// Get a collection of possible choices
		GameStatesCollection states;
		GetChoiceCollection(initialState, depth, states);
		
		if (states.empty())
			return model->EvaluateState(initialState);
		
		--depth;
		
		// Maximize the minimum outcome
		GameState* state = states.front();
		states.pop_front();
		int best = Min(state, depth);
		delete state;
		
		while (!states.empty()) {
			state = states.front();
			states.pop_front();
			int v = Min(state, depth);
			delete state;
			if (v > best) best = v;
		}
		return best;
	}
};

#endif // DECISION_MAKER_INCLUDED

