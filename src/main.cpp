#include <iostream>
#include <string>

#include "database/database.h"
#include "parser/sql_parser.h"
#include "parser/token.h"
#include "parser/lexer.h"
#include "parser/ast.h"
#include "executor/executor.h"

using namespace std;

int main()
{
    Database db;
    Executor executor(db);
    string query;

    while (true)
    {
        cout << "MiniDB > ";

        getline(cin, query);

        if (query == "EXIT")
            break;

        Lexer lexer(query);

        auto tokens = lexer.tokenize();

        SQLParser parser(tokens);

        try
        {
            auto stmt = parser.parse();

            executor.execute(stmt.get());
        }
        catch (const exception &e)
        {
            cout << "Error: " << e.what() << endl;
        }
    }

    return 0;
}