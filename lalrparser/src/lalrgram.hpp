#ifndef LALRGRAM_HPP
#define LALRGRAM_HPP

#include "gram.hpp"
#include "lalrparsingtable.hpp"

#include <deque>
#include <utility>

/// @todo: consider priorities for productions, additional to ignore
/// @todo: consider moving ignore flag from Information to ItemCore,
///		since ignore items and non-ignore items can be mixed together in same info.
///		An item is ignorable if production is an ignore production
///		or item was added to info during closure calculation.
template<typename T, T N = T(0)> class LalrGrammar: public Grammar<T>
{
public:
	typedef LalrParsingTable<T> Table;
	typedef typename Table::Action Action;
	typedef typename Grammar<T>::ProductionNo ProductionNo;
	typedef typename Grammar<T>::Set Set;
	typedef typename Grammar<T>::Token Token;
	typedef typename Grammar<T>::String String;
	typedef typename Grammar<T>::ProductionIndex ProductionIndex;

	struct Error
	{
		Error(size_t state, T tok, Action a1, Action a2)
			: state(state), token(tok), actions(a1, a2) {}
		size_t state;
		T token;
		std::pair<Action, Action> actions;
	};

	static const T endToken;

	void clear();
	Table createParsingTable();

	const std::vector<Error>& getErrors() const;
	const std::set<ProductionNo>& getAmbiguousProductions() const;

	bool isIgnoreProduction(ProductionNo prodNo) const;
	/// @return true if action shifts into ignore state or reduces with ignore production.
	bool isIgnorableAction(const Action& action) const;

	/// Production with position in body.
	struct ItemCore
	{
		ItemCore(const ProductionNo& p, size_t d, bool ignore)
			: productionNo(p), dot(d), ignore(ignore) {}
		bool operator<(const ItemCore& other) const;
		bool operator==(const ItemCore& other) const;

		const ProductionNo productionNo;
		const size_t dot;
		bool ignore;
	};

	typedef std::map<ItemCore, Set> InfoItems;
	typedef typename InfoItems::value_type Item;

	/// Information consists of item cores with set of tokens.
	/// The tokens are follows of body of production.
	/// An information corresponds to a state of the parser.
	struct Information
	{
		Information()/* : ignore(false)*/ {}

		InfoItems items;
		/// Flag of being info of ignore production.
		//bool ignore;
		/// @return true if all items are ignorable.
		bool isIgnorable() const;
		/// Adds follow tokens to each item.
		void addFollow(const Set&);
	};

	/// @return information corresponding to state in parsing table.
	/// @see createParsingTable()
	const Information& getInformation(size_t state) const;

	/// @return number of informations
	size_t getNumberOfStates() const;

protected:

	/// Information with id.
	struct TaggedInfo
	{
		TaggedInfo(const Information& info, size_t id)
			: info(info), id(id) {}
		Information info;
		size_t id;
	};

	struct InfoCoreCompare
	{
		bool operator()(const TaggedInfo&, const TaggedInfo&) const;
	};

	typedef std::set<TaggedInfo,InfoCoreCompare> Collection;
	typedef std::vector<typename Collection::iterator> Informations;
	typedef	std::vector<std::map<Token,size_t> > GotoInformations;

	struct CoreCompare {
		bool operator()(const Item&, const Item&) const;
	};

	static bool core_equal(const Information&,const Information&);
	static bool info_union_add(Information&, const Information&);

	/// Fills ignoreStartInfo with all start items of all ignore productions.
	/// Has to be called before startProduction is added.
	void calcIgnoreStartInfo();
	Information& calcClosure(Information&, Set& infoFirsts) const;
	Information& closure(Information&) const;
	void calcCollection();
	std::pair<size_t, bool> addInfo(Information&);

	bool isStartProduction(ProductionNo prodNo) const;

	/// Handles conflicts between actions of different ignorable state.
	/// Handles problems with ignore productions:
	///		A->B C;
	///		A->d;
	///		B->@;
	///		@->e;
	/// Now parsing "ed" gets stuck at "e.d" in item [A->B.C], since item [B->., e] would be preferred over [@->e.],
	/// since [@->e.] is ignorable and [B->., e] not.
	/// So this function recognizes this special case and chooses [@->e.] for resolution of this conflict.
	/// @param a1 first conflicting action and at the same time destination to write more prior action to.
	/// @param a2 second conflicting action.
	/// @param a2Ingorable ignorable state of action a2 and implicitly the inverse of ignorable state of action a1.
	/// @return new ignorable state of final action a1.
	//bool handleConflict(Action& a1, const Action& a2, bool a2Ignorable) const;

	Collection collection;
	Informations informations;
	GotoInformations gotos;

	/// All start items of all ignore productions.
	/// The follows just contain all firsts and have to be completed for each insert of ignore-start-items.
	Information ignoreStartInfo;
	Set ignoreFirsts;

	std::vector<Error> errors;
	std::set<ProductionNo> ambiguousProductions;
};

#endif
