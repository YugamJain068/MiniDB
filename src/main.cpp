// #include <iostream>
// #include <string>

// #include "database/database.h"
// #include "parser/sql_parser.h"
// #include "parser/token.h"
// #include "parser/lexer.h"
// #include "parser/ast.h"
// #include "executor/executor.h"
// #include "storage/page.h"
// #include "buffer/buffer_pool_manager.h"

// using namespace std;

// int main()
// {
//     Database db;
//     Executor executor(db);
//     string query;
//     BufferPoolManager bpm(3);
// bpm.FetchPage("../data/users.tbl",0);

// bpm.UnpinPage(0);

// bpm.FetchPage("../data/users.tbl",1);

// bpm.UnpinPage(1);

// bpm.FetchPage("../data/users.tbl",2);

// bpm.UnpinPage(2);

// bpm.FetchPage("../data/users.tbl",0);

// bpm.UnpinPage(0);

// bpm.FetchPage("../data/users.tbl",3);


//     while (true)
//     {
//         cout << "MiniDB > ";

//         getline(cin, query);

//         if (query == "EXIT")
//             break;

//         Lexer lexer(query);

//         auto tokens = lexer.tokenize();

//         SQLParser parser(tokens);

//         try
//         {
//             auto stmt = parser.parse();

//             executor.execute(stmt.get());
//         }
//         catch (const exception &e)
//         {
//             cout << "Error: " << e.what() << endl;
//         }
//     }

//     return 0;
// }


#include <iostream>

#include "buffer/buffer_pool_manager.h"

using namespace std;

int main()
{
    BufferPoolManager bpm(3);

    cout << "========== TEST 1 : Load Pages ==========\n";

    bpm.FetchPage("../data/users.tbl", 0);
    bpm.UnpinPage(0);

    bpm.FetchPage("../data/users.tbl", 1);
    bpm.UnpinPage(1);

    bpm.FetchPage("../data/users.tbl", 2);
    bpm.UnpinPage(2);

    bpm.printFrames();
    bpm.printPageTable();
    bpm.printLRU();

    cout << "\n========== TEST 2 : Cache Hit ==========\n";

    bpm.FetchPage("../data/users.tbl", 0);
    bpm.UnpinPage(0);

    bpm.printFrames();
    bpm.printLRU();

    cout << "\n========== TEST 3 : Dirty Page ==========\n";

    bpm.MarkDirty(0);

    bpm.printFrames();

    cout << "\n========== TEST 4 : Flush Page ==========\n";

    bpm.FlushPage("../data/users.tbl", 0);

    bpm.printFrames();

    cout << "\n========== TEST 5 : LRU Replacement ==========\n";

    bpm.FetchPage("../data/users.tbl", 3);

    bpm.printFrames();
    bpm.printPageTable();
    bpm.printLRU();

    cout << "\n========== TEST 6 : Dirty Eviction ==========\n";

    bpm.MarkDirty(2);
    bpm.UnpinPage(3);

    bpm.FetchPage("../data/users.tbl", 4);

    bpm.printFrames();
    bpm.printPageTable();
    bpm.printLRU();

    cout << "\n========== TEST 7 : Flush All ==========\n";

    bpm.FlushAllPages("../data/users.tbl");

    bpm.printFrames();

    cout << "\n========== ALL TESTS COMPLETED ==========\n";

    return 0;
}