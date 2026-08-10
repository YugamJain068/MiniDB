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
// #include "storage/page.h"

// using namespace std;

// int main()
// {
//     Database db;
//     Executor executor(db);
//     string query;

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

// #include <iostream>

// #include "buffer/buffer_pool_manager.h"

// using namespace std;

// int main()
// {
//     BufferPoolManager bpm(3);

//     cout << "========== TEST 1 : Load Pages ==========\n";

//     bpm.FetchPage("../data/users.tbl", 0);
//     bpm.UnpinPage(0);

//     bpm.FetchPage("../data/users.tbl", 1);
//     bpm.UnpinPage(1);

//     bpm.FetchPage("../data/users.tbl", 2);
//     bpm.UnpinPage(2);

//     bpm.printFrames();
//     bpm.printPageTable();
//     bpm.printLRU();

//     cout << "\n========== TEST 2 : Cache Hit ==========\n";

//     bpm.FetchPage("../data/users.tbl", 0);
//     bpm.UnpinPage(0);

//     bpm.printFrames();
//     bpm.printLRU();

//     cout << "\n========== TEST 3 : Dirty Page ==========\n";

//     bpm.MarkDirty(0);

//     bpm.printFrames();

//     cout << "\n========== TEST 4 : Flush Page ==========\n";

//     bpm.FlushPage("../data/users.tbl", 0);

//     bpm.printFrames();

//     cout << "\n========== TEST 5 : LRU Replacement ==========\n";

//     bpm.FetchPage("../data/users.tbl", 3);

//     bpm.printFrames();
//     bpm.printPageTable();
//     bpm.printLRU();

//     cout << "\n========== TEST 6 : Dirty Eviction ==========\n";

//     bpm.MarkDirty(2);
//     bpm.UnpinPage(3);

//     bpm.FetchPage("../data/users.tbl", 4);

//     bpm.printFrames();
//     bpm.printPageTable();
//     bpm.printLRU();

//     cout << "\n========== TEST 7 : Flush All ==========\n";

//     bpm.FlushAllPages("../data/users.tbl");

//     bpm.printFrames();

//     cout << "\n========== ALL TESTS COMPLETED ==========\n";

//     return 0;
// }

// #include <iostream>
// #include <vector>
// #include <cstdlib>
// #include <ctime>

// #include "buffer/buffer_pool_manager.h"
// #include "bptree/b_plus_tree.h"

// int main()
// {
//     std::srand(static_cast<unsigned>(std::time(nullptr)));

//     BufferPoolManager bpm(10);

//     BPlusTree tree(&bpm);

//     std::vector<int> keys = {
//         10,20,30,40,50,
//         60,70,80,90,100,
//         110,120,130
//     };

//     std::cout << "========== INSERT ==========\n";

//     for(int key : keys)
//     {
//         RecordPointer ptr(
//             rand() % 20,      // pageId
//             rand() % 100      // slotId
//         );

//         tree.insert(key, ptr);

//         std::cout
//             << "Inserted "
//             << key
//             << " -> ("
//             << ptr.pageId
//             << ", "
//             << ptr.slotId
//             << ")\n";
//     }

//     std::cout << "\n========== TREE ==========\n";

//     tree.printTree();

//     std::cout << "\n========== LEAF CHAIN ==========\n";

//     tree.printLeaves();

//     std::cout << "\n========== SEARCH ==========\n";

//     std::vector<int> searchKeys = {
//         10,
//         30,
//         60,
//         70,
//         100,
//         130,
//         25,
//         200
//     };

//     for(int key : searchKeys)
//     {
//         RecordPointer ptr = tree.search(key);

//         if(ptr.pageId == -1)
//         {
//             std::cout
//                 << key
//                 << " -> Not Found\n";
//         }
//         else
//         {
//             std::cout
//                 << key
//                 << " -> ("
//                 << ptr.pageId
//                 << ", "
//                 << ptr.slotId
//                 << ")\n";
//         }
//     }

//     return 0;
// }

// int main()
// {
//     Page page;

//     Row r1;
//     r1.id = 1;
//     strcpy(r1.name, "Alice");

//     Row r2;
//     r2.id = 2;
//     strcpy(r2.name, "Bob");

//     page.insertRow(r1);
//     page.insertRow(r2);

//     Row a = page.getRow(0);
//     Row b = page.getRow(1);

//     std::cout << a.id << " " << a.name << '\n';
//     std::cout << b.id << " " << b.name << '\n';
//     return 0;
// }

// #include <iostream>
// #include <vector>
// #include <algorithm>
// #include <numeric>
// #include <random>

// #include "buffer/buffer_pool_manager.h"
// #include "bptree/b_plus_tree.h"

// bool sequentialTest()
// {
//     std::cout
//         << "\n========== SEQUENTIAL INSERT ==========\n";

//     BufferPoolManager bpm(10);
//     BPlusTree tree(&bpm);

//     const int N = 200;

//     for (int i = 1; i <= N; i++)
//     {
//         RecordPointer ptr(
//             i / 100,
//             i);

//         tree.insert(i, ptr);
//     }

//     std::cout
//         << "\nLeaf Chain:\n";

//     tree.printLeaves();

//     for (int i = 1; i <= N; i++)
//     {
//         RecordPointer ptr =
//             tree.search(i);

//         if (ptr.pageId != i / 100 ||
//             ptr.slotId != i)
//         {
//             std::cout
//                 << "FAIL: Key "
//                 << i
//                 << '\n';

//             std::cout
//                 << "Expected: ("
//                 << i / 100
//                 << ", "
//                 << i
//                 << ")\n";

//             std::cout
//                 << "Found: ("
//                 << ptr.pageId
//                 << ", "
//                 << ptr.slotId
//                 << ")\n";

//             return false;
//         }
//     }

//     std::cout
//         << "\nSequential Search: PASS\n";

//     return true;
// }

// bool randomInsertTest()
// {
//     std::cout
//         << "\n========== RANDOM INSERT ==========\n";

//     BufferPoolManager bpm(10);
//     BPlusTree tree(&bpm);

//     const int N = 200;

//     std::vector<int> keys(N);

//     std::iota(
//         keys.begin(),
//         keys.end(),
//         1);

//     std::mt19937 rng(42);

//     std::shuffle(
//         keys.begin(),
//         keys.end(),
//         rng);

//     for (int key : keys)
//     {
//         RecordPointer ptr(
//             key / 100,
//             key);

//         tree.insert(
//             key,
//             ptr);
//     }

//     std::cout
//         << "\nLeaf Chain:\n";

//     tree.printLeaves();

//     for (int i = 1; i <= N; i++)
//     {
//         RecordPointer ptr =
//             tree.search(i);

//         if (ptr.pageId != i / 100 ||
//             ptr.slotId != i)
//         {
//             std::cout
//                 << "FAIL: Key "
//                 << i
//                 << '\n';

//             std::cout
//                 << "Expected: ("
//                 << i / 100
//                 << ", "
//                 << i
//                 << ")\n";

//             std::cout
//                 << "Found: ("
//                 << ptr.pageId
//                 << ", "
//                 << ptr.slotId
//                 << ")\n";

//             return false;
//         }
//     }

//     std::cout
//         << "\nRandom Search: PASS\n";

//     return true;
// }

// bool missingKeyTest()
// {
//     std::cout
//         << "\n========== MISSING KEY TEST ==========\n";

//     BufferPoolManager bpm(10);
//     BPlusTree tree(&bpm);

//     const int N = 200;

//     for (int i = 1; i <= N; i++)
//     {
//         tree.insert(
//             i,
//             RecordPointer(
//                 i / 100,
//                 i));
//     }

//     for (int i = 201; i <= 300; i++)
//     {
//         RecordPointer ptr =
//             tree.search(i);

//         if (ptr.pageId != -1)
//         {
//             std::cout
//                 << "FAIL: Unexpected key "
//                 << i
//                 << " found\n";

//             return false;
//         }
//     }

//     std::cout
//         << "Missing Key Search: PASS\n";

//     return true;
// }

// int main()
// {
//     bool sequential =
//         sequentialTest();

//     bool random =
//         randomInsertTest();

//     bool missing =
//         missingKeyTest();

//     std::cout
//         << "\n========================================\n";

//     std::cout
//         << "TEST SUMMARY\n";

//     std::cout
//         << "Sequential Insert : "
//         << (sequential ? "PASS" : "FAIL")
//         << '\n';

//     std::cout
//         << "Random Insert     : "
//         << (random ? "PASS" : "FAIL")
//         << '\n';

//     std::cout
//         << "Missing Keys      : "
//         << (missing ? "PASS" : "FAIL")
//         << '\n';

//     std::cout
//         << "========================================\n";

//     if (sequential &&
//         random &&
//         missing)
//     {
//         std::cout
//             << "ALL TESTS PASSED\n";
//     }
//     else
//     {
//         std::cout
//             << "SOME TESTS FAILED\n";
//     }

//     return 0;
// }

// #include <iostream>
// #include <vector>
// #include <algorithm>
// #include <numeric>
// #include <random>

// #include "buffer/buffer_pool_manager.h"
// #include "bptree/b_plus_tree.h"


// // ============================================================
// // Sequential Insert + Internal Split Test
// // ============================================================

// bool sequentialTest()
// {
//     std::cout
//         << "\n========================================\n"
//         << "SEQUENTIAL INTERNAL SPLIT TEST\n"
//         << "========================================\n";

//     BufferPoolManager bpm(10);

//     BPlusTree tree(&bpm);

//     const int N = 1000;

//     // --------------------------------------------------------
//     // Insert
//     // --------------------------------------------------------

//     for (int i = 1; i <= N; i++)
//     {
//         tree.insert(
//             i,
//             RecordPointer(
//                 i / 100,
//                 i));
//     }

//     std::cout
//         << "\nTree Height: "
//         << tree.height()
//         << '\n';

//     tree.printRootInfo();

//     // --------------------------------------------------------
//     // Search verification
//     // --------------------------------------------------------

//     for (int i = 1; i <= N; i++)
//     {
//         RecordPointer ptr =
//             tree.search(i);

//         if (ptr.pageId != i / 100 ||
//             ptr.slotId != i)
//         {
//             std::cout
//                 << "\nSEARCH FAILURE\n";

//             std::cout
//                 << "Key: "
//                 << i
//                 << '\n';

//             std::cout
//                 << "Expected: ("
//                 << i / 100
//                 << ", "
//                 << i
//                 << ")\n";

//             std::cout
//                 << "Found: ("
//                 << ptr.pageId
//                 << ", "
//                 << ptr.slotId
//                 << ")\n";

//             return false;
//         }
//     }

//     std::cout
//         << "Search: PASS\n";

//     // --------------------------------------------------------
//     // Parent pointer verification
//     // --------------------------------------------------------

//     if (!tree.verifyParentPointers(tree.getRoot()))
//     {
//         std::cout
//             << "Parent pointers: FAIL\n";

//         return false;
//     }

//     std::cout
//         << "Parent pointers: PASS\n";

//     // --------------------------------------------------------
//     // Internal structure verification
//     // --------------------------------------------------------

//     if (!tree.verifyInternalStructure(tree.getRoot()))
//     {
//         std::cout
//             << "Internal structure: FAIL\n";

//         return false;
//     }

//     std::cout
//         << "Internal structure: PASS\n";

//     return true;
// }


// // ============================================================
// // Random Insert + Internal Split Test
// // ============================================================

// bool randomTest()
// {
//     std::cout
//         << "\n========================================\n"
//         << "RANDOM INTERNAL SPLIT TEST\n"
//         << "========================================\n";

//     BufferPoolManager bpm(10);

//     // IMPORTANT:
//     // This is a completely new tree.

//     BPlusTree tree(&bpm);

//     const int N = 1000;

//     std::vector<int> keys(N);

//     std::iota(
//         keys.begin(),
//         keys.end(),
//         1);

//     std::mt19937 rng(42);

//     std::shuffle(
//         keys.begin(),
//         keys.end(),
//         rng);

//     // --------------------------------------------------------
//     // Insert random order
//     // --------------------------------------------------------

//     for (int key : keys)
//     {
//         tree.insert(
//             key,
//             RecordPointer(
//                 key / 100,
//                 key));
//     }

//     std::cout
//         << "\nTree Height: "
//         << tree.height()
//         << '\n';

//     tree.printRootInfo();

//     // --------------------------------------------------------
//     // Search all keys
//     // --------------------------------------------------------

//     for (int i = 1; i <= N; i++)
//     {
//         RecordPointer ptr =
//             tree.search(i);

//         if (ptr.pageId != i / 100 ||
//             ptr.slotId != i)
//         {
//             std::cout
//                 << "\nSEARCH FAILURE\n";

//             std::cout
//                 << "Key: "
//                 << i
//                 << '\n';

//             std::cout
//                 << "Expected: ("
//                 << i / 100
//                 << ", "
//                 << i
//                 << ")\n";

//             std::cout
//                 << "Found: ("
//                 << ptr.pageId
//                 << ", "
//                 << ptr.slotId
//                 << ")\n";

//             return false;
//         }
//     }

//     std::cout
//         << "Search: PASS\n";

//     // --------------------------------------------------------
//     // Parent pointers
//     // --------------------------------------------------------

//     if (!tree.verifyParentPointers(tree.getRoot()))
//     {
//         std::cout
//             << "Parent pointers: FAIL\n";

//         return false;
//     }

//     std::cout
//         << "Parent pointers: PASS\n";

//     // --------------------------------------------------------
//     // Internal structure
//     // --------------------------------------------------------

//     if (!tree.verifyInternalStructure(tree.getRoot()))
//     {
//         std::cout
//             << "Internal structure: FAIL\n";

//         return false;
//     }

//     std::cout
//         << "Internal structure: PASS\n";

//     return true;
// }


// // ============================================================
// // Main
// // ============================================================

// int main()
// {
//     bool sequential =
//         sequentialTest();

//     bool random =
//         randomTest();

//     std::cout
//         << "\n========================================\n"
//         << "DAY 3 TEST SUMMARY\n"
//         << "========================================\n";

//     std::cout
//         << "Sequential Internal Split : "
//         << (sequential ? "PASS" : "FAIL")
//         << '\n';

//     std::cout
//         << "Random Internal Split     : "
//         << (random ? "PASS" : "FAIL")
//         << '\n';

//     if (sequential && random)
//     {
//         std::cout
//             << "\nALL DAY 3 TESTS PASSED\n";
//     }
//     else
//     {
//         std::cout
//             << "\nDAY 3 TEST FAILED\n";
//     }

//     return 0;
// }

// #include <iostream>
// #include <vector>
// #include <algorithm>
// #include <numeric>
// #include <random>

// #include "buffer/buffer_pool_manager.h"
// #include "bptree/b_plus_tree.h"

// int main()
// {
//     BufferPoolManager bpm(10);

//     BPlusTree tree(&bpm);

//     const int N = 10000;

//     std::vector<int> keys(N);

//     std::iota(
//         keys.begin(),
//         keys.end(),
//         1);

//     std::mt19937 rng(42);

//     std::shuffle(
//         keys.begin(),
//         keys.end(),
//         rng);

//     std::cout
//         << "===== DAY 4 STRESS TEST =====\n";

//     for (int key : keys)
//     {
//         tree.insert(
//             key,
//             RecordPointer(
//                 key / 100,
//                 key));
//     }

//     std::cout
//         << "\nTree Height: "
//         << tree.height()
//         << '\n';

//     std::cout
//         << "Tree Verification: ";

//     if (tree.verifyTree())
//         std::cout << "PASS\n";
//     else
//         std::cout << "FAIL\n";

//     // Search verification
//     for (int i = 1; i <= N; i++)
//     {
//         RecordPointer ptr =
//             tree.search(i);

//         if (ptr.pageId != i / 100 ||
//             ptr.slotId != i)
//         {
//             std::cout
//                 << "SEARCH FAILED: "
//                 << i
//                 << '\n';

//             return 1;
//         }
//     }

//     std::cout
//         << "Search Verification: PASS\n";

//     // Missing-key verification
//     for (int i = N + 1;
//          i <= N + 1000;
//          i++)
//     {
//         RecordPointer ptr =
//             tree.search(i);

//         if (ptr.pageId != -1)
//         {
//             std::cout
//                 << "MISSING KEY TEST FAILED: "
//                 << i
//                 << '\n';

//             return 1;
//         }
//     }

//     std::cout
//         << "Missing Key Verification: PASS\n";

//     std::cout
//         << "\nALL DAY 4 TESTS PASSED\n";

//     return 0;
// }

#include <iostream>

#include "bptree/b_plus_tree_page.h"
#include "bptree/b_plus_tree_leaf_page.h"
#include "bptree/b_plus_tree_internal_page.h"

int main()
{
    std::cout
        << "Header: "
        << sizeof(BPlusTreePageHeader)
        << '\n';

    std::cout
        << "LeafEntry: "
        << sizeof(LeafEntry)
        << '\n';

    std::cout
        << "LeafPage: "
        << sizeof(LeafPage)
        << '\n';

    std::cout
        << "InternalEntry: "
        << sizeof(InternalEntry)
        << '\n';

    std::cout
        << "InternalPage: "
        << sizeof(InternalPage)
        << '\n';
}