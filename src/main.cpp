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

// #include <iostream>

// #include "bptree/b_plus_tree_page.h"
// #include "bptree/b_plus_tree_leaf_page.h"
// #include "bptree/b_plus_tree_internal_page.h"

// int main()
// {
//     std::cout
//         << "Header: "
//         << sizeof(BPlusTreePageHeader)
//         << '\n';

//     std::cout
//         << "LeafEntry: "
//         << sizeof(LeafEntry)
//         << '\n';

//     std::cout
//         << "LeafPage: "
//         << sizeof(LeafPage)
//         << '\n';

//     std::cout
//         << "InternalEntry: "
//         << sizeof(InternalEntry)
//         << '\n';

//     std::cout
//         << "InternalPage: "
//         << sizeof(InternalPage)
//         << '\n';
// }

// #include <iostream>

// #include "bptree/b_plus_tree_leaf_page.h"
// #include "bptree/b_plus_tree_internal_page.h"

// int main()
// {
//     LeafPage leaf{};

//     initializeLeafPage(
//         leaf,
//         10);

//     std::cout
//         << "Leaf Page ID: "
//         << leaf.header.pageId
//         << '\n';

//     std::cout
//         << "Leaf Type: "
//         << static_cast<int>(
//                leaf.header.type)
//         << '\n';

//     std::cout
//         << "Leaf Size: "
//         << leaf.header.size
//         << '\n';

//     std::cout
//         << "Next Page: "
//         << leaf.nextPageId
//         << '\n';

//     InternalPage internal{};

//     initializeInternalPage(
//         internal,
//         20);

//     std::cout
//         << "\nInternal Page ID: "
//         << internal.header.pageId
//         << '\n';

//     std::cout
//         << "Internal Type: "
//         << static_cast<int>(
//                internal.header.type)
//         << '\n';

//     std::cout
//         << "Internal Size: "
//         << internal.header.size
//         << '\n';

//     return 0;
// }

// #include <iostream>

// #include "bptree/b_plus_tree_page_io.h"
// #include "storage/page.h"
// #include "storage/storage_manager.h"

// int main()
// {
//     // =========================================
//     // Create leaf page
//     // =========================================

//     LeafPage leaf{};

//     initializeLeafPage(
//         leaf,
//         10);

//     leaf.header.size = 3;

//     leaf.entries[0].key = 10;
//     leaf.entries[0].value =
//         RecordPointer(1, 5);

//     leaf.entries[1].key = 20;
//     leaf.entries[1].value =
//         RecordPointer(1, 6);

//     leaf.entries[2].key = 30;
//     leaf.entries[2].value =
//         RecordPointer(2, 7);

//     leaf.nextPageId = 20;

//     // =========================================
//     // Serialize
//     // =========================================

//     Page page;

//     serializeLeafPage(
//         leaf,
//         page);

//     // =========================================
//     // Deserialize
//     // =========================================

//     LeafPage restored{};

//     deserializeLeafPage(
//         page,
//         restored);

//     // =========================================
//     // Verify
//     // =========================================

//     std::cout
//         << "Page ID: "
//         << restored.header.pageId
//         << '\n';

//     std::cout
//         << "Type: "
//         << static_cast<int>(
//                restored.header.type)
//         << '\n';

//     std::cout
//         << "Size: "
//         << restored.header.size
//         << '\n';

//     std::cout
//         << "Next Page: "
//         << restored.nextPageId
//         << '\n';

//     for (int i = 0;
//          i < restored.header.size;
//          i++)
//     {
//         std::cout
//             << restored.entries[i].key
//             << " -> ("
//             << restored.entries[i].value.pageId
//             << ", "
//             << restored.entries[i].value.slotId
//             << ")\n";
//     }

//     bool passed = true;

//     if (restored.header.pageId != 10)
//         passed = false;

//     if (restored.header.size != 3)
//         passed = false;

//     if (restored.nextPageId != 20)
//         passed = false;

//     if (restored.entries[0].key != 10)
//         passed = false;

//     if (restored.entries[1].key != 20)
//         passed = false;

//     if (restored.entries[2].key != 30)
//         passed = false;

//     if (passed)
//         std::cout << "\nLeaf serialization: PASS\n";
//     else
//         std::cout << "\nLeaf serialization: FAIL\n";

//     InternalPage internal{};

//     initializeInternalPage(
//         internal,
//         20);

//     internal.header.size = 2;

//     internal.firstChildPageId = 10;

//     internal.entries[0].key = 50;
//     internal.entries[0].childPageId = 30;

//     internal.entries[1].key = 100;
//     internal.entries[1].childPageId = 40;
//     Page internalPage;

//     serializeInternalPage(
//         internal,
//         internalPage);

//     InternalPage restoredInternal{};

//     deserializeInternalPage(
//         internalPage,
//         restoredInternal);

//     std::cout
//         << "Internal Page ID: "
//         << restoredInternal.header.pageId
//         << '\n';

//     std::cout
//         << "First Child: "
//         << restoredInternal.firstChildPageId
//         << '\n';

//     for (int i = 0;
//          i < restoredInternal.header.size;
//          i++)
//     {
//         std::cout
//             << restoredInternal.entries[i].key
//             << " -> Page "
//             << restoredInternal.entries[i].childPageId
//             << '\n';
//     }

//     StorageManager storage;

//     const std::string filename =
//         "../data/test_index.idx";

//     int pageId =
//         storage.allocatePage(filename);

//     LeafPage leaf2{};

//     initializeLeafPage(
//         leaf2,
//         pageId);

//     leaf2.header.size = 2;

//     leaf2.entries[0].key = 100;
//     leaf2.entries[0].value =
//         RecordPointer(5, 10);

//     leaf2.entries[1].key = 200;
//     leaf2.entries[1].value =
//         RecordPointer(5, 11);

//     leaf2.nextPageId = -1;

//     Page page2;

//     serializeLeafPage(
//         leaf2,
//         page2);

//     storage.writePage(
//         filename,
//         pageId,
//         page2);

//     Page loaded =
//         storage.readPage(
//             filename,
//             pageId);

//     LeafPage restored2{};

//     deserializeLeafPage(
//         loaded,
//         restored2);

//     if (restored2.entries[0].key == 100 &&
//         restored2.entries[1].key == 200 &&
//         restored2.entries[0].value.pageId == 5 &&
//         restored2.entries[0].value.slotId == 10)
//     {
//         std::cout
//             << "Disk persistence: PASS\n";
//     }
//     else
//     {
//         std::cout
//             << "Disk persistence: FAIL\n";
//     }

//     return 0;
// }

// #include <iostream>

// #include "buffer/buffer_pool_manager.h"
// #include "bptree/b_plus_tree_page_manager.h"

// int main()
// {
//     BufferPoolManager bpm(2);

//     BPlusTreePageManager manager(
//         bpm,
//         "../data/test_index.idx");

//     // =====================================
//     // Allocate leaf
//     // =====================================

//     int leafPageId =
//         manager.allocateLeafPage();

//     std::cout
//         << "Leaf Page: "
//         << leafPageId
//         << '\n';

//     // =====================================
//     // Read
//     // =====================================

//     LeafPage leaf{};

//     if (!manager.readLeafPage(
//             leafPageId,
//             leaf))
//     {
//         std::cout
//             << "Read failed\n";

//         return 1;
//     }

//     std::cout
//         << "Initial size: "
//         << leaf.header.size
//         << '\n';

//     // =====================================
//     // Modify
//     // =====================================

//     leaf.header.size = 2;

//     leaf.entries[0].key = 100;
//     leaf.entries[0].value =
//         RecordPointer(5, 10);

//     leaf.entries[1].key = 200;
//     leaf.entries[1].value =
//         RecordPointer(5, 11);

//     leaf.nextPageId = -1;

//     // =====================================
//     // Write
//     // =====================================

//     if (!manager.writeLeafPage(
//             leafPageId,
//             leaf))
//     {
//         std::cout
//             << "Write failed\n";

//         return 1;
//     }

//     // =====================================
//     // Read again
//     // =====================================

//     LeafPage restored{};

//     manager.readLeafPage(
//         leafPageId,
//         restored);

//     // =====================================
//     // Verify
//     // =====================================

//     bool passed =
//         restored.header.size == 2 &&
//         restored.entries[0].key == 100 &&
//         restored.entries[1].key == 200 &&
//         restored.entries[0].value.pageId == 5 &&
//         restored.entries[0].value.slotId == 10 &&
//         restored.entries[1].value.pageId == 5 &&
//         restored.entries[1].value.slotId == 11;

//     if (passed)
//     {
//         std::cout
//             << "Buffer Pool B+ Tree Page Test: PASS\n";
//     }
//     else
//     {
//         std::cout
//             << "Buffer Pool B+ Tree Page Test: FAIL\n";
//     }

//     bpm.FlushAllPages();

//     return 0;
// }

// #include <iostream>

// #include "bptree/b_plus_tree_metadata_io.h"

// int main()
// {
//     BPlusTreeMetadata metadata{};

//     initializeMetadata(
//         metadata);

//     metadata.rootPageId = 10;
//     metadata.pageCount = 11;

//     Page page;

//     serializeMetadata(
//         metadata,
//         page);

//     BPlusTreeMetadata restored{};

//     deserializeMetadata(
//         page,
//         restored);

//     bool passed =
//         restored.magic ==
//             BPLUS_INDEX_MAGIC &&
//         restored.version ==
//             BPLUS_INDEX_VERSION &&
//         restored.rootPageId == 10 &&
//         restored.pageCount == 11;

//     if (passed)
//     {
//         std::cout
//             << "Metadata serialization: PASS\n";
//     }
//     else
//     {
//         std::cout
//             << "Metadata serialization: FAIL\n";
//     }

//     std::cout
//         << "Magic: "
//         << restored.magic
//         << '\n';

//     std::cout
//         << "Version: "
//         << restored.version
//         << '\n';

//     std::cout
//         << "Root Page: "
//         << restored.rootPageId
//         << '\n';

//     std::cout
//         << "Page Count: "
//         << restored.pageCount
//         << '\n';

//     return 0;
// }

// #include <iostream>

// #include "buffer/buffer_pool_manager.h"
// #include "bptree/b_plus_tree_page_manager.h"

// int main()
// {
//     const std::string filename =
//         "../data/day8_test.idx";

//     BufferPoolManager bpm(2);

//     BPlusTreePageManager manager(
//         bpm,
//         filename);

//     if (!manager.initializeIndex())
//     {
//         std::cout
//             << "Index initialization: FAIL\n";

//         return 1;
//     }

//     BPlusTreeMetadata metadata{};

//     if (!manager.readMetadata(metadata))
//     {
//         std::cout
//             << "Metadata read: FAIL\n";

//         return 1;
//     }

//     std::cout
//         << "Root Page: "
//         << metadata.rootPageId
//         << '\n';

//     std::cout
//         << "Page Count: "
//         << metadata.pageCount
//         << '\n';

//     if (metadata.rootPageId == -1)
//     {
//         std::cout
//             << "Empty index: PASS\n";
//     }

//     // Allocate first leaf
//     int leafPage =
//         manager.allocateLeafPage();

//     std::cout
//         << "Leaf Page: "
//         << leafPage
//         << '\n';

//     metadata.rootPageId =
//         leafPage;

//     metadata.pageCount =
//         bpm.GetPageCount(filename);

//     manager.writeMetadata(
//         metadata);

//     BPlusTreeMetadata restored{};

//     manager.readMetadata(
//         restored);

//     if (restored.rootPageId ==
//             leafPage)
//     {
//         std::cout
//             << "Root persistence: PASS\n";
//     }
//     else
//     {
//         std::cout
//             << "Root persistence: FAIL\n";
//     }

//     bpm.FlushAllPages();

//     return 0;
// }

// #include <iostream>

// #include "buffer/buffer_pool_manager.h"
// #include "bptree/b_plus_tree.h"

// int main()
// {
//     const std::string filename =
//         "../data/day9_test.idx";

//     BufferPoolManager bpm(3);

//     BPlusTree tree(
//         &bpm,
//         filename);

//     if (!tree.initialize())
//     {
//         std::cout
//             << "Initialization: FAIL\n";

//         return 1;
//     }

//     std::cout
//         << "Initial root: "
//         << tree.getRootPageId()
//         << '\n';

//     if (!tree.empty())
//     {
//         std::cout
//             << "Empty tree: FAIL\n";

//         return 1;
//     }

//     if (!tree.insert(
//             100,
//             RecordPointer(5, 10)))
//     {
//         std::cout
//             << "First insert: FAIL\n";

//         return 1;
//     }

//     std::cout
//         << "Root after insert: "
//         << tree.getRootPageId()
//         << '\n';

//     if (tree.getRootPageId() != 1)
//     {
//         std::cout
//             << "Root page test: FAIL\n";

//         return 1;
//     }

//     LeafPage leaf{};

//     if (!tree.readRootLeaf(leaf))
//     {
//         std::cout
//             << "Read root: FAIL\n";

//         return 1;
//     }

//     if (leaf.header.size != 1 ||
//         leaf.entries[0].key != 100 ||
//         leaf.entries[0].value.pageId != 5 ||
//         leaf.entries[0].value.slotId != 10)
//     {
//         std::cout
//             << "Root contents: FAIL\n";

//         return 1;
//     }

//     std::cout
//         << "First persistent insert: PASS\n";

//     bpm.FlushAllPages();

//     return 0;
// }

// #include <iostream>
// #include <vector>

// #include "buffer/buffer_pool_manager.h"
// #include "bptree/b_plus_tree.h"

// int main()
// {
//     const std::string filename =
//         "../data/day10_test.idx";

//     BufferPoolManager bpm(3);

//     BPlusTree tree(
//         &bpm,
//         filename);

//     if (!tree.initialize())
//     {
//         std::cout
//             << "Initialization: FAIL\n";

//         return 1;
//     }

//     // =====================================
//     // Insert
//     // =====================================

//     std::vector<int> keys = {
//         50,
//         10,
//         40,
//         20,
//         30
//     };

//     for (int key : keys)
//     {
//         bool result =
//             tree.insert(
//                 key,
//                 RecordPointer(
//                     key / 10,
//                     key));

//         if (!result)
//         {
//             std::cout
//                 << "Insert failed: "
//                 << key
//                 << '\n';

//             return 1;
//         }
//     }

//     // =====================================
//     // Print
//     // =====================================

//     tree.printRootLeaf();

//     // =====================================
//     // Search
//     // =====================================

//     bool passed = true;

//     for (int key : keys)
//     {
//         RecordPointer ptr =
//             tree.search(key);

//         if (ptr.pageId != key / 10 ||
//             ptr.slotId != key)
//         {
//             std::cout
//                 << "SEARCH FAILURE: "
//                 << key
//                 << '\n';

//             passed = false;

//             break;
//         }
//     }

//     if (passed)
//     {
//         std::cout
//             << "Search: PASS\n";
//     }

//     // =====================================
//     // Missing key
//     // =====================================

//     RecordPointer missing =
//         tree.search(999);

//     if (missing.pageId == -1)
//     {
//         std::cout
//             << "Missing key: PASS\n";
//     }
//     else
//     {
//         std::cout
//             << "Missing key: FAIL\n";

//         passed = false;
//     }

//     // =====================================
//     // Duplicate
//     // =====================================

//     if (!tree.insert(
//             30,
//             RecordPointer(99, 99)))
//     {
//         std::cout
//             << "Duplicate key: PASS\n";
//     }
//     else
//     {
//         std::cout
//             << "Duplicate key: FAIL\n";

//         passed = false;
//     }

//     // =====================================
//     // Final result
//     // =====================================

//     if (passed)
//     {
//         std::cout
//             << "\nDAY 10 TEST: PASS\n";
//     }
//     else
//     {
//         std::cout
//             << "\nDAY 10 TEST: FAIL\n";
//     }

//     bpm.FlushAllPages();

//     return 0;
// }

// #include <iostream>

// #include "buffer/buffer_pool_manager.h"
// #include "bptree/b_plus_tree.h"

// int main()
// {
//     const std::string filename =
//         "../data/day10_test.idx";

//     BufferPoolManager bpm(3);

//     BPlusTree tree(
//         &bpm,
//         filename);

//     if (!tree.initialize())
//     {
//         std::cout
//             << "Restart: FAIL\n";

//         return 1;
//     }

//     std::vector<int> keys = {
//         10,
//         20,
//         30,
//         40,
//         50
//     };

//     bool passed = true;

//     for (int key : keys)
//     {
//         RecordPointer ptr =
//             tree.search(key);

//         if (ptr.pageId != key / 10 ||
//             ptr.slotId != key)
//         {
//             passed = false;

//             std::cout
//                 << "Recovery failure: "
//                 << key
//                 << '\n';

//             break;
//         }
//     }

//     if (passed)
//     {
//         std::cout
//             << "Restart search: PASS\n";
//     }

//     return 0;
// }

#include <iostream>
#include <cstdio>

#include "buffer/buffer_pool_manager.h"
#include "bptree/b_plus_tree.h"

int main()
{
    const std::string filename =
        "../data/day11_test.idx";

    // =========================================================
    // IMPORTANT:
    // Start with a fresh index file.
    // =========================================================

    std::remove(filename.c_str());

    std::cout
        << "========================================\n";

    std::cout
        << "DAY 11 - PERSISTENT LEAF SPLIT TEST\n";

    std::cout
        << "========================================\n";


    // =========================================================
    // Create Buffer Pool
    // =========================================================

    BufferPoolManager bpm(5);

    BPlusTree tree(
        &bpm,
        filename);


    // =========================================================
    // Initialize Index
    // =========================================================

    if (!tree.initialize())
    {
        std::cout
            << "Initialization: FAIL\n";

        return 1;
    }

    std::cout
        << "Initialization: PASS\n";


    // =========================================================
    // Insert LEAF_MAX_ENTRIES + 1 keys
    //
    // LEAF_MAX_ENTRIES = 339
    //
    // Therefore:
    //
    // 1 ... 340
    //
    // 340th insertion should trigger split.
    // =========================================================

    const int N =
        LEAF_MAX_ENTRIES + 1;

    bool passed = true;

    for (int i = 1;
         i <= N;
         i++)
    {
        bool result =
            tree.insert(
                i,
                RecordPointer(
                    i / 100,
                    i));

        if (!result)
        {
            std::cout
                << "Insert failed at key "
                << i
                << '\n';

            passed = false;

            break;
        }
    }

    if (!passed)
    {
        std::cout
            << "Sequential insertion: FAIL\n";

        return 1;
    }

    std::cout
        << "Sequential insertion: PASS\n";


    // =========================================================
    // Read LEFT leaf
    // =========================================================

    LeafPage leftLeaf{};

    if (!tree.readRootLeaf(
            leftLeaf))
    {
        std::cout
            << "Read left leaf: FAIL\n";

        return 1;
    }


    // =========================================================
    // Root should still be the left leaf
    //
    // Day 11 does NOT create internal root yet.
    // =========================================================

    int leftPageId =
        tree.getRootPageId();

    int rightPageId =
        leftLeaf.nextPageId;


    std::cout
        << "\nLeft Page ID: "
        << leftPageId
        << '\n';

    std::cout
        << "Right Page ID: "
        << rightPageId
        << '\n';


    // =========================================================
    // Verify right leaf exists
    // =========================================================

    if (rightPageId == -1)
    {
        std::cout
            << "Leaf split occurred: FAIL\n";

        return 1;
    }

    std::cout
        << "Leaf split occurred: PASS\n";


    // =========================================================
    // Read RIGHT leaf
    // =========================================================

    LeafPage rightLeaf{};

    if (!tree.readLeafPage(
            rightPageId,
            rightLeaf))
    {
        std::cout
            << "Read right leaf: FAIL\n";

        return 1;
    }

    std::cout
        << "Right leaf read: PASS\n";


    // =========================================================
    // Print sizes
    // =========================================================

    std::cout
        << "\nLeft Leaf Size: "
        << leftLeaf.header.size
        << '\n';

    std::cout
        << "Right Leaf Size: "
        << rightLeaf.header.size
        << '\n';

    std::cout
        << "Total Entries: "
        << leftLeaf.header.size
           + rightLeaf.header.size
        << '\n';


    // =========================================================
    // Verify total entries
    // =========================================================

    if (leftLeaf.header.size +
            rightLeaf.header.size
        != N)
    {
        std::cout
            << "Total entry count: FAIL\n";

        passed = false;
    }
    else
    {
        std::cout
            << "Total entry count: PASS\n";
    }


    // =========================================================
    // Verify both leaves are non-empty
    // =========================================================

    if (leftLeaf.header.size == 0 ||
        rightLeaf.header.size == 0)
    {
        std::cout
            << "Non-empty leaves: FAIL\n";

        passed = false;
    }
    else
    {
        std::cout
            << "Non-empty leaves: PASS\n";
    }


    // =========================================================
    // Verify leaf chain
    //
    // Left -> Right -> nullptr
    // =========================================================

    if (leftLeaf.nextPageId != rightPageId)
    {
        std::cout
            << "Leaf chain left -> right: FAIL\n";

        passed = false;
    }
    else
    {
        std::cout
            << "Leaf chain left -> right: PASS\n";
    }


    if (rightLeaf.nextPageId != -1)
    {
        std::cout
            << "Leaf chain termination: FAIL\n";

        passed = false;
    }
    else
    {
        std::cout
            << "Leaf chain termination: PASS\n";
    }


    // =========================================================
    // Verify key ordering inside LEFT leaf
    // =========================================================

    for (int i = 1;
         i < leftLeaf.header.size;
         i++)
    {
        if (leftLeaf.entries[i - 1].key >=
            leftLeaf.entries[i].key)
        {
            std::cout
                << "Left leaf ordering: FAIL\n";

            passed = false;

            break;
        }
    }

    if (passed)
    {
        std::cout
            << "Left leaf ordering: PASS\n";
    }


    // =========================================================
    // Verify key ordering inside RIGHT leaf
    // =========================================================

    bool rightSorted = true;

    for (int i = 1;
         i < rightLeaf.header.size;
         i++)
    {
        if (rightLeaf.entries[i - 1].key >=
            rightLeaf.entries[i].key)
        {
            rightSorted = false;
            break;
        }
    }

    if (!rightSorted)
    {
        std::cout
            << "Right leaf ordering: FAIL\n";

        passed = false;
    }
    else
    {
        std::cout
            << "Right leaf ordering: PASS\n";
    }


    // =========================================================
    // Verify boundary
    //
    // left last key < right first key
    // =========================================================

    int leftLastKey =
        leftLeaf
            .entries[
                leftLeaf.header.size - 1]
            .key;

    int rightFirstKey =
        rightLeaf
            .entries[0]
            .key;

    std::cout
        << "\nLeft Last Key: "
        << leftLastKey
        << '\n';

    std::cout
        << "Right First Key: "
        << rightFirstKey
        << '\n';


    if (leftLastKey >= rightFirstKey)
    {
        std::cout
            << "Leaf boundary ordering: FAIL\n";

        passed = false;
    }
    else
    {
        std::cout
            << "Leaf boundary ordering: PASS\n";
    }


    // =========================================================
    // Verify all keys
    //
    // Left should contain first part.
    // Right should contain second part.
    // =========================================================

    int expectedKey = 1;

    bool keysCorrect = true;


    // LEFT

    for (int i = 0;
         i < leftLeaf.header.size;
         i++)
    {
        if (leftLeaf.entries[i].key !=
            expectedKey)
        {
            keysCorrect = false;

            std::cout
                << "Key mismatch in LEFT leaf\n";

            std::cout
                << "Expected: "
                << expectedKey
                << " Found: "
                << leftLeaf.entries[i].key
                << '\n';

            break;
        }

        expectedKey++;
    }


    // RIGHT

    if (keysCorrect)
    {
        for (int i = 0;
             i < rightLeaf.header.size;
             i++)
        {
            if (rightLeaf.entries[i].key !=
                expectedKey)
            {
                keysCorrect = false;

                std::cout
                    << "Key mismatch in RIGHT leaf\n";

                std::cout
                    << "Expected: "
                    << expectedKey
                    << " Found: "
                    << rightLeaf.entries[i].key
                    << '\n';

                break;
            }

            expectedKey++;
        }
    }


    if (!keysCorrect)
    {
        std::cout
            << "All keys present: FAIL\n";

        passed = false;
    }
    else
    {
        std::cout
            << "All keys present: PASS\n";
    }


    // =========================================================
    // Verify RecordPointers
    // =========================================================

    bool pointersCorrect = true;

    expectedKey = 1;


    for (int i = 0;
         i < leftLeaf.header.size;
         i++)
    {
        const RecordPointer& ptr =
            leftLeaf.entries[i].value;

        if (ptr.slotId != expectedKey)
        {
            pointersCorrect = false;
            break;
        }

        expectedKey++;
    }


    if (pointersCorrect)
    {
        for (int i = 0;
             i < rightLeaf.header.size;
             i++)
        {
            const RecordPointer& ptr =
                rightLeaf.entries[i].value;

            if (ptr.slotId != expectedKey)
            {
                pointersCorrect = false;
                break;
            }

            expectedKey++;
        }
    }


    if (!pointersCorrect)
    {
        std::cout
            << "RecordPointer validation: FAIL\n";

        passed = false;
    }
    else
    {
        std::cout
            << "RecordPointer validation: PASS\n";
    }


    // =========================================================
    // Print split
    // =========================================================

    std::cout
        << "\n========== SPLIT RESULT ==========\n";

    std::cout
        << "Left:  [ ";

    for (int i = 0;
         i < leftLeaf.header.size;
         i++)
    {
        std::cout
            << leftLeaf.entries[i].key
            << " ";
    }

    std::cout
        << "]\n";

    std::cout
        << "Right: [ ";

    for (int i = 0;
         i < rightLeaf.header.size;
         i++)
    {
        std::cout
            << rightLeaf.entries[i].key
            << " ";
    }

    std::cout
        << "]\n";


    // =========================================================
    // Flush to disk
    // =========================================================

    bpm.FlushAllPages();

    std::cout
        << "\nDisk flush completed.\n";


    // =========================================================
    // Final in-process result
    // =========================================================

    if (passed)
    {
        std::cout
            << "\n========================================\n"
            << "DAY 11 IN-MEMORY TEST: PASS\n"
            << "========================================\n";
    }
    else
    {
        std::cout
            << "\n========================================\n"
            << "DAY 11 IN-MEMORY TEST: FAIL\n"
            << "========================================\n";

        return 1;
    }


    return 0;
}