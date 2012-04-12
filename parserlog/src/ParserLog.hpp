#ifndef PARSERLOG_HPP
#define PARSERLOG_HPP

#include "../../genutils/src/codestream.hpp"

#include <vector>
#include <string>
#include <utility>

class ParserLog
{
public:
	struct Node
	{
		Node(int type) : type(type) {}
		virtual ~Node() {}
		virtual const std::string& getToken() const = 0;
		virtual const std::vector<Node*>& getChilds() const = 0;
		int getType() const { return type; }
	protected:
		int type;
	};

	ParserLog();
	~ParserLog();
	void shift(int token, int size, const char* str);
	void reduce(int head, int bodysize);
	void reset();
	const Node* root() const { return stack.empty() ? 0 : stack.back(); }
	int stackSize() const { return (int)stack.size(); }

private:
	
	

	struct NonTerminalNode : public Node
	{
		NonTerminalNode(int type) : Node(type) {}
		NonTerminalNode(int type, int size, Node** childs) : Node(type), childs(childs, childs + size) { }
		~NonTerminalNode() { for (size_t c = 0; c < childs.size(); ++c) delete childs[c]; }
		const std::string& getToken() const { static std::string empty; return empty; }
		const std::vector<Node*>& getChilds() const { return childs; }
		std::vector<Node*> childs;
	};

	struct TerminalNode : public Node
	{
		TerminalNode(int type) : Node(type) {}
		TerminalNode(int type, int size, const char* token) : Node(type), token(token, size) {}
		const std::string& getToken() const { return token; }
		const std::vector<Node*>& getChilds() const { static std::vector<Node*> empty; return empty; }
		std::string token;
	};

	std::vector<Node*> stack;
	Node* next;
};

#endif
