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

// #include <iostream>
// #include <cstdio>

// #include "buffer/buffer_pool_manager.h"
// #include "bptree/b_plus_tree.h"

// int main()
// {
//     const std::string filename =
//         "../data/day11_test.idx";

//     // =========================================================
//     // IMPORTANT:
//     // Start with a fresh index file.
//     // =========================================================

//     std::remove(filename.c_str());

//     std::cout
//         << "========================================\n";

//     std::cout
//         << "DAY 11 - PERSISTENT LEAF SPLIT TEST\n";

//     std::cout
//         << "========================================\n";

//     // =========================================================
//     // Create Buffer Pool
//     // =========================================================

//     BufferPoolManager bpm(5);

//     BPlusTree tree(
//         &bpm,
//         filename);

//     // =========================================================
//     // Initialize Index
//     // =========================================================

//     if (!tree.initialize())
//     {
//         std::cout
//             << "Initialization: FAIL\n";

//         return 1;
//     }

//     std::cout
//         << "Initialization: PASS\n";

//     // =========================================================
//     // Insert LEAF_MAX_ENTRIES + 1 keys
//     //
//     // LEAF_MAX_ENTRIES = 339
//     //
//     // Therefore:
//     //
//     // 1 ... 340
//     //
//     // 340th insertion should trigger split.
//     // =========================================================

//     const int N =
//         LEAF_MAX_ENTRIES + 1;

//     bool passed = true;

//     for (int i = 1;
//          i <= N;
//          i++)
//     {
//         bool result =
//             tree.insert(
//                 i,
//                 RecordPointer(
//                     i / 100,
//                     i));

//         if (!result)
//         {
//             std::cout
//                 << "Insert failed at key "
//                 << i
//                 << '\n';

//             passed = false;

//             break;
//         }
//     }

//     if (!passed)
//     {
//         std::cout
//             << "Sequential insertion: FAIL\n";

//         return 1;
//     }

//     std::cout
//         << "Sequential insertion: PASS\n";

//     // =========================================================
//     // Read LEFT leaf
//     // =========================================================

//     LeafPage leftLeaf{};

//     if (!tree.readRootLeaf(
//             leftLeaf))
//     {
//         std::cout
//             << "Read left leaf: FAIL\n";

//         return 1;
//     }

//     // =========================================================
//     // Root should still be the left leaf
//     //
//     // Day 11 does NOT create internal root yet.
//     // =========================================================

//     int leftPageId =
//         tree.getRootPageId();

//     int rightPageId =
//         leftLeaf.nextPageId;

//     std::cout
//         << "\nLeft Page ID: "
//         << leftPageId
//         << '\n';

//     std::cout
//         << "Right Page ID: "
//         << rightPageId
//         << '\n';

//     // =========================================================
//     // Verify right leaf exists
//     // =========================================================

//     if (rightPageId == -1)
//     {
//         std::cout
//             << "Leaf split occurred: FAIL\n";

//         return 1;
//     }

//     std::cout
//         << "Leaf split occurred: PASS\n";

//     // =========================================================
//     // Read RIGHT leaf
//     // =========================================================

//     LeafPage rightLeaf{};

//     if (!tree.readLeafPage(
//             rightPageId,
//             rightLeaf))
//     {
//         std::cout
//             << "Read right leaf: FAIL\n";

//         return 1;
//     }

//     std::cout
//         << "Right leaf read: PASS\n";

//     // =========================================================
//     // Print sizes
//     // =========================================================

//     std::cout
//         << "\nLeft Leaf Size: "
//         << leftLeaf.header.size
//         << '\n';

//     std::cout
//         << "Right Leaf Size: "
//         << rightLeaf.header.size
//         << '\n';

//     std::cout
//         << "Total Entries: "
//         << leftLeaf.header.size
//            + rightLeaf.header.size
//         << '\n';

//     // =========================================================
//     // Verify total entries
//     // =========================================================

//     if (leftLeaf.header.size +
//             rightLeaf.header.size
//         != N)
//     {
//         std::cout
//             << "Total entry count: FAIL\n";

//         passed = false;
//     }
//     else
//     {
//         std::cout
//             << "Total entry count: PASS\n";
//     }

//     // =========================================================
//     // Verify both leaves are non-empty
//     // =========================================================

//     if (leftLeaf.header.size == 0 ||
//         rightLeaf.header.size == 0)
//     {
//         std::cout
//             << "Non-empty leaves: FAIL\n";

//         passed = false;
//     }
//     else
//     {
//         std::cout
//             << "Non-empty leaves: PASS\n";
//     }

//     // =========================================================
//     // Verify leaf chain
//     //
//     // Left -> Right -> nullptr
//     // =========================================================

//     if (leftLeaf.nextPageId != rightPageId)
//     {
//         std::cout
//             << "Leaf chain left -> right: FAIL\n";

//         passed = false;
//     }
//     else
//     {
//         std::cout
//             << "Leaf chain left -> right: PASS\n";
//     }

//     if (rightLeaf.nextPageId != -1)
//     {
//         std::cout
//             << "Leaf chain termination: FAIL\n";

//         passed = false;
//     }
//     else
//     {
//         std::cout
//             << "Leaf chain termination: PASS\n";
//     }

//     // =========================================================
//     // Verify key ordering inside LEFT leaf
//     // =========================================================

//     for (int i = 1;
//          i < leftLeaf.header.size;
//          i++)
//     {
//         if (leftLeaf.entries[i - 1].key >=
//             leftLeaf.entries[i].key)
//         {
//             std::cout
//                 << "Left leaf ordering: FAIL\n";

//             passed = false;

//             break;
//         }
//     }

//     if (passed)
//     {
//         std::cout
//             << "Left leaf ordering: PASS\n";
//     }

//     // =========================================================
//     // Verify key ordering inside RIGHT leaf
//     // =========================================================

//     bool rightSorted = true;

//     for (int i = 1;
//          i < rightLeaf.header.size;
//          i++)
//     {
//         if (rightLeaf.entries[i - 1].key >=
//             rightLeaf.entries[i].key)
//         {
//             rightSorted = false;
//             break;
//         }
//     }

//     if (!rightSorted)
//     {
//         std::cout
//             << "Right leaf ordering: FAIL\n";

//         passed = false;
//     }
//     else
//     {
//         std::cout
//             << "Right leaf ordering: PASS\n";
//     }

//     // =========================================================
//     // Verify boundary
//     //
//     // left last key < right first key
//     // =========================================================

//     int leftLastKey =
//         leftLeaf
//             .entries[
//                 leftLeaf.header.size - 1]
//             .key;

//     int rightFirstKey =
//         rightLeaf
//             .entries[0]
//             .key;

//     std::cout
//         << "\nLeft Last Key: "
//         << leftLastKey
//         << '\n';

//     std::cout
//         << "Right First Key: "
//         << rightFirstKey
//         << '\n';

//     if (leftLastKey >= rightFirstKey)
//     {
//         std::cout
//             << "Leaf boundary ordering: FAIL\n";

//         passed = false;
//     }
//     else
//     {
//         std::cout
//             << "Leaf boundary ordering: PASS\n";
//     }

//     // =========================================================
//     // Verify all keys
//     //
//     // Left should contain first part.
//     // Right should contain second part.
//     // =========================================================

//     int expectedKey = 1;

//     bool keysCorrect = true;

//     // LEFT

//     for (int i = 0;
//          i < leftLeaf.header.size;
//          i++)
//     {
//         if (leftLeaf.entries[i].key !=
//             expectedKey)
//         {
//             keysCorrect = false;

//             std::cout
//                 << "Key mismatch in LEFT leaf\n";

//             std::cout
//                 << "Expected: "
//                 << expectedKey
//                 << " Found: "
//                 << leftLeaf.entries[i].key
//                 << '\n';

//             break;
//         }

//         expectedKey++;
//     }

//     // RIGHT

//     if (keysCorrect)
//     {
//         for (int i = 0;
//              i < rightLeaf.header.size;
//              i++)
//         {
//             if (rightLeaf.entries[i].key !=
//                 expectedKey)
//             {
//                 keysCorrect = false;

//                 std::cout
//                     << "Key mismatch in RIGHT leaf\n";

//                 std::cout
//                     << "Expected: "
//                     << expectedKey
//                     << " Found: "
//                     << rightLeaf.entries[i].key
//                     << '\n';

//                 break;
//             }

//             expectedKey++;
//         }
//     }

//     if (!keysCorrect)
//     {
//         std::cout
//             << "All keys present: FAIL\n";

//         passed = false;
//     }
//     else
//     {
//         std::cout
//             << "All keys present: PASS\n";
//     }

//     // =========================================================
//     // Verify RecordPointers
//     // =========================================================

//     bool pointersCorrect = true;

//     expectedKey = 1;

//     for (int i = 0;
//          i < leftLeaf.header.size;
//          i++)
//     {
//         const RecordPointer& ptr =
//             leftLeaf.entries[i].value;

//         if (ptr.slotId != expectedKey)
//         {
//             pointersCorrect = false;
//             break;
//         }

//         expectedKey++;
//     }

//     if (pointersCorrect)
//     {
//         for (int i = 0;
//              i < rightLeaf.header.size;
//              i++)
//         {
//             const RecordPointer& ptr =
//                 rightLeaf.entries[i].value;

//             if (ptr.slotId != expectedKey)
//             {
//                 pointersCorrect = false;
//                 break;
//             }

//             expectedKey++;
//         }
//     }

//     if (!pointersCorrect)
//     {
//         std::cout
//             << "RecordPointer validation: FAIL\n";

//         passed = false;
//     }
//     else
//     {
//         std::cout
//             << "RecordPointer validation: PASS\n";
//     }

//     // =========================================================
//     // Print split
//     // =========================================================

//     std::cout
//         << "\n========== SPLIT RESULT ==========\n";

//     std::cout
//         << "Left:  [ ";

//     for (int i = 0;
//          i < leftLeaf.header.size;
//          i++)
//     {
//         std::cout
//             << leftLeaf.entries[i].key
//             << " ";
//     }

//     std::cout
//         << "]\n";

//     std::cout
//         << "Right: [ ";

//     for (int i = 0;
//          i < rightLeaf.header.size;
//          i++)
//     {
//         std::cout
//             << rightLeaf.entries[i].key
//             << " ";
//     }

//     std::cout
//         << "]\n";

//     // =========================================================
//     // Flush to disk
//     // =========================================================

//     bpm.FlushAllPages();

//     std::cout
//         << "\nDisk flush completed.\n";

//     // =========================================================
//     // Final in-process result
//     // =========================================================

//     if (passed)
//     {
//         std::cout
//             << "\n========================================\n"
//             << "DAY 11 IN-MEMORY TEST: PASS\n"
//             << "========================================\n";
//     }
//     else
//     {
//         std::cout
//             << "\n========================================\n"
//             << "DAY 11 IN-MEMORY TEST: FAIL\n"
//             << "========================================\n";

//         return 1;
//     }

//     return 0;
// }

// #include <iostream>
// #include <cstdio>

// #include "buffer/buffer_pool_manager.h"
// #include "bptree/b_plus_tree.h"
// #include "bptree/b_plus_tree_internal_page.h"
// #include "bptree/b_plus_tree_leaf_page.h"
// #include "storage/record_pointer.h"

// int main()
// {
//     const std::string filename =
//         "../data/day12_test.idx";

//     // ==========================================
//     // Clean old test file
//     // ==========================================

//     std::remove(filename.c_str());

//     std::cout
//         << "\n========================================\n"
//         << "DAY 12: ROOT CREATION TEST\n"
//         << "========================================\n\n";

//     // ==========================================
//     // Buffer Pool
//     // ==========================================

//     BufferPoolManager bpm(10);

//     // ==========================================
//     // B+ Tree
//     // ==========================================

//     BPlusTree tree(
//         &bpm,
//         filename);

//     if (!tree.initialize())
//     {
//         std::cout
//             << "Tree initialization: FAIL\n";

//         return 1;
//     }

//     std::cout
//         << "Tree initialization: PASS\n";

//     // ==========================================
//     // Initial Root
//     // ==========================================

//     int initialRoot =
//         tree.getRootPageId();

//     std::cout
//         << "Initial Root Page ID: "
//         << initialRoot
//         << '\n';

//     // ==========================================
//     // Insert enough keys to force
//     // one leaf split
//     // ==========================================

//     const int totalKeys = 340;

//     bool insertSuccess = true;

//     for (int key = 1;
//          key <= totalKeys;
//          key++)
//     {
//         RecordPointer pointer;

//         pointer.pageId = key;
//         pointer.slotId = key;

//         if (!tree.insert(
//                 key,
//                 pointer))
//         {
//             std::cout
//                 << "Insert failed for key: "
//                 << key
//                 << '\n';

//             insertSuccess = false;
//             break;
//         }
//     }

//     std::cout
//         << "Inserted "
//         << totalKeys
//         << " keys: "
//         << (insertSuccess
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     if (!insertSuccess)
//     {
//         return 1;
//     }

//     // ==========================================
//     // Root should now be INTERNAL
//     // ==========================================

//     int rootPageId =
//         tree.getRootPageId();

//     std::cout
//         << "\nRoot Page ID: "
//         << rootPageId
//         << '\n';

//     // ==========================================
//     // Read root page
//     // ==========================================

//     Page* rootPage =
//         bpm.FetchPage(
//             filename,
//             rootPageId);

//     if (rootPage == nullptr)
//     {
//         std::cout
//             << "Root page fetch: FAIL\n";

//         return 1;
//     }

//     std::cout
//         << "Root page fetch: PASS\n";

//     // ==========================================
//     // Deserialize Internal Page
//     // ==========================================

//     InternalPage root{};

//     deserializeInternalPage(
//         *rootPage,
//         root);

//     bpm.UnpinPage(
//         filename,
//         rootPageId);

//     // ==========================================
//     // Root information
//     // ==========================================

//     std::cout
//         << "\n========== ROOT INFORMATION ==========\n";

//     std::cout
//         << "Root Page ID: "
//         << rootPageId
//         << '\n';

//     std::cout
//         << "Root Size: "
//         << root.header.size
//         << '\n';

//     std::cout
//         << "First Child Page ID: "
//         << root.firstChildPageId
//         << '\n';

//     if (root.header.size > 0)
//     {
//         std::cout
//             << "Separator Key: "
//             << root.entries[0].key
//             << '\n';

//         std::cout
//             << "Right Child Page ID: "
//             << root.entries[0].childPageId
//             << '\n';
//     }

//     // ==========================================
//     // Validate root size
//     // ==========================================

//     bool rootSizeCorrect =
//         root.header.size == 1;

//     std::cout
//         << "\nRoot entry count: "
//         << (rootSizeCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Validate left child
//     // ==========================================

//     bool leftChildCorrect =
//     root.firstChildPageId == 1;

//     std::cout
//         << "Left child pointer: "
//         << (leftChildCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Validate separator
//     //
//     // With 340 entries and a split at 170:
//     //
//     // Left  = 1 ... 170
//     // Right = 171 ... 340
//     //
//     // Separator = 171
//     // ==========================================

//     bool separatorCorrect =
//         root.header.size > 0 &&
//         root.entries[0].key == 171;

//     std::cout
//         << "Separator key: "
//         << (separatorCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Validate right child
//     // ==========================================

//     bool rightChildCorrect =
//     root.header.size > 0 &&
//     root.entries[0].childPageId == 2;

//     std::cout
//         << "Right child pointer: "
//         << (rightChildCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Read both leaves
//     // ==========================================

//     LeafPage leftLeaf{};
//     LeafPage rightLeaf{};

//     bool leftRead =
//         tree.readLeafPage(
//             root.firstChildPageId,
//             leftLeaf);

//     bool rightRead = false;

//     int rightPageId = -1;

//     if (root.header.size > 0)
//     {
//         rightPageId =
//             root.entries[0].childPageId;

//         rightRead =
//             tree.readLeafPage(
//                 rightPageId,
//                 rightLeaf);
//     }

//     std::cout
//         << "\nLeft leaf read: "
//         << (leftRead
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "Right leaf read: "
//         << (rightRead
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Validate leaf contents
//     // ==========================================

//     bool leftSizeCorrect =
//         leftRead &&
//         leftLeaf.header.size == 170;

//     bool rightSizeCorrect =
//         rightRead &&
//         rightLeaf.header.size == 170;

//     std::cout
//         << "Left leaf size: "
//         << (leftSizeCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "Right leaf size: "
//         << (rightSizeCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Validate boundary
//     // ==========================================

//     bool boundaryCorrect =
//         leftRead &&
//         rightRead &&
//         leftLeaf.entries[
//             leftLeaf.header.size - 1
//         ].key == 170 &&
//         rightLeaf.entries[0].key == 171;

//     std::cout
//         << "Leaf boundary: "
//         << (boundaryCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Validate leaf chain
//     // ==========================================

//     bool chainCorrect =
//         leftRead &&
//         rightRead &&
//         leftLeaf.nextPageId == rightPageId &&
//         rightLeaf.nextPageId == -1;

//     std::cout
//         << "Leaf chain: "
//         << (chainCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Print structure
//     // ==========================================

//     std::cout
//         << "\n========== TREE STRUCTURE ==========\n\n";

//     std::cout
//         << "                 Root: "
//         << rootPageId
//         << "\n"
//         << "                ["
//         << (root.header.size > 0
//                 ? root.entries[0].key
//                 : -1)
//         << "]\n"
//         << "               /   \\\n"
//         << "              /     \\\n"
//         << "         Leaf: "
//         << root.firstChildPageId
//         << "    Leaf: "
//         << rightPageId
//         << '\n';

//     std::cout
//         << "        [1..170]   [171..340]\n";

//     // ==========================================
//     // Final result
//     // ==========================================

//     bool allPassed =
//         insertSuccess &&
//         rootSizeCorrect &&
//         leftChildCorrect &&
//         separatorCorrect &&
//         rightChildCorrect &&
//         leftSizeCorrect &&
//         rightSizeCorrect &&
//         boundaryCorrect &&
//         chainCorrect;
//     std::cout << "\n========== VALIDATION DEBUG ==========\n";

// std::cout << "insertSuccess       = "
//           << insertSuccess << '\n';

// std::cout << "rootSizeCorrect     = "
//           << rootSizeCorrect << '\n';

// std::cout << "leftChildCorrect    = "
//           << leftChildCorrect << '\n';

// std::cout << "separatorCorrect    = "
//           << separatorCorrect << '\n';

// std::cout << "rightChildCorrect   = "
//           << rightChildCorrect << '\n';

// std::cout << "leftSizeCorrect     = "
//           << leftSizeCorrect << '\n';

// std::cout << "rightSizeCorrect    = "
//           << rightSizeCorrect << '\n';

// std::cout << "boundaryCorrect     = "
//           << boundaryCorrect << '\n';

// std::cout << "chainCorrect        = "
//           << chainCorrect << '\n';

// std::cout << "Initial Root        = "
//           << initialRoot << '\n';

// std::cout << "Root first child    = "
//           << root.firstChildPageId << '\n';

// std::cout << "Right child         = "
//           << rightPageId << '\n';

// std::cout << "Separator           = "
//           << root.entries[0].key << '\n';

//     std::cout
//         << "\n========================================\n";

//     if (allPassed)
//     {
//         std::cout
//             << "DAY 12 ROOT CREATION: PASS\n";
//     }
//     else
//     {
//         std::cout
//             << "DAY 12 ROOT CREATION: FAIL\n";
//     }

//     std::cout
//         << "========================================\n\n";

//     // ==========================================
//     // Flush everything
//     // ==========================================

//     bpm.FlushAllPages();

//     std::cout
//         << "\nDisk flush completed.\n";

//     return allPassed ? 0 : 1;
// }

// #include <iostream>
// #include <cstdio>

// #include "buffer/buffer_pool_manager.h"
// #include "bptree/b_plus_tree.h"
// #include "bptree/b_plus_tree_internal_page.h"
// #include "bptree/b_plus_tree_leaf_page.h"
// #include "storage/record_pointer.h"

// int main()
// {
//     const std::string filename =
//         "../data/day13_test.idx";

//     // ==========================================
//     // Remove old test file
//     // ==========================================

//     std::remove(filename.c_str());

//     std::cout
//         << "\n========================================\n"
//         << "DAY 13: MULTIPLE LEAF SPLITS TEST\n"
//         << "========================================\n\n";

//     // ==========================================
//     // Buffer Pool
//     // ==========================================

//     BufferPoolManager bpm(10);

//     // ==========================================
//     // B+ Tree
//     // ==========================================

//     BPlusTree tree(
//         &bpm,
//         filename);

//     if (!tree.initialize())
//     {
//         std::cout
//             << "Tree initialization: FAIL\n";

//         return 1;
//     }

//     std::cout
//         << "Tree initialization: PASS\n";

//     // ==========================================
//     // Insert 1 -> 510
//     //
//     // 170 entries per leaf
//     //
//     // Expected:
//     //
//     // Leaf 1 = 1..170
//     // Leaf 2 = 171..340
//     // Leaf 3 = 341..510
//     //
//     // Root:
//     //
//     //          [171 | 341]
//     //         /     |     \
//     //        L1     L2     L3
//     // ==========================================

//     const int totalKeys = 510;

//     bool insertSuccess = true;

//     for (int key = 1;
//          key <= totalKeys;
//          key++)
//     {
//         RecordPointer pointer;

//         pointer.pageId = key;
//         pointer.slotId = key;

//         if (!tree.insert(
//                 key,
//                 pointer))
//         {
//             std::cout
//                 << "Insert failed for key: "
//                 << key
//                 << '\n';

//             insertSuccess = false;
//             break;
//         }
//     }

//     std::cout
//         << "\nInserted "
//         << totalKeys
//         << " keys: "
//         << (insertSuccess
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     if (!insertSuccess)
//     {
//         return 1;
//     }

//     // ==========================================
//     // Root
//     // ==========================================

//     int rootPageId =
//         tree.getRootPageId();

//     std::cout
//         << "\nRoot Page ID: "
//         << rootPageId
//         << '\n';

//     // ==========================================
//     // Read root
//     // ==========================================

//     InternalPage root{};

//     bool rootRead =
//         tree.readInternalPage(
//             rootPageId,
//             root);

//     std::cout
//         << "Root read: "
//         << (rootRead
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     if (!rootRead)
//     {
//         return 1;
//     }

//     // ==========================================
//     // Print root
//     // ==========================================

//     std::cout
//         << "\n========== ROOT INFORMATION ==========\n";

//     std::cout
//         << "Root Page ID: "
//         << rootPageId
//         << '\n';

//     std::cout
//         << "Root Size: "
//         << root.header.size
//         << '\n';

//     std::cout
//         << "First Child Page ID: "
//         << root.firstChildPageId
//         << '\n';

//     for (int i = 0;
//          i < root.header.size;
//          i++)
//     {
//         std::cout
//             << "Entry "
//             << i
//             << ": Key = "
//             << root.entries[i].key
//             << ", Child = "
//             << root.entries[i].childPageId
//             << '\n';
//     }

//     // ==========================================
//     // Root validation
//     // ==========================================

//     bool rootSizeCorrect =
//         root.header.size == 2;

//     bool firstSeparatorCorrect =
//         root.header.size >= 2 &&
//         root.entries[0].key == 171;

//     bool secondSeparatorCorrect =
//         root.header.size >= 2 &&
//         root.entries[1].key == 341;

//     bool firstChildValid =
//         root.firstChildPageId != -1;

//     bool secondChildValid =
//         root.header.size >= 1 &&
//         root.entries[0].childPageId != -1;

//     bool thirdChildValid =
//         root.header.size >= 2 &&
//         root.entries[1].childPageId != -1;

//     std::cout
//         << "\nRoot size: "
//         << (rootSizeCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "First separator (171): "
//         << (firstSeparatorCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "Second separator (341): "
//         << (secondSeparatorCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "First child pointer: "
//         << (firstChildValid
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "Second child pointer: "
//         << (secondChildValid
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "Third child pointer: "
//         << (thirdChildValid
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Read all three leaves
//     // ==========================================

//     int leaf1PageId =
//         root.firstChildPageId;

//     int leaf2PageId =
//         root.entries[0].childPageId;

//     int leaf3PageId =
//         root.entries[1].childPageId;

//     LeafPage leaf1{};
//     LeafPage leaf2{};
//     LeafPage leaf3{};

//     bool leaf1Read =
//         tree.readLeafPage(
//             leaf1PageId,
//             leaf1);

//     bool leaf2Read =
//         tree.readLeafPage(
//             leaf2PageId,
//             leaf2);

//     bool leaf3Read =
//         tree.readLeafPage(
//             leaf3PageId,
//             leaf3);

//     std::cout
//         << "\n========== LEAF READ ==========\n";

//     std::cout
//         << "Leaf 1 read: "
//         << (leaf1Read
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "Leaf 2 read: "
//         << (leaf2Read
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "Leaf 3 read: "
//         << (leaf3Read
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Leaf sizes
//     // ==========================================

//     bool leaf1SizeCorrect =
//         leaf1Read &&
//         leaf1.header.size == 170;

//     bool leaf2SizeCorrect =
//         leaf2Read &&
//         leaf2.header.size == 170;

//     bool leaf3SizeCorrect =
//         leaf3Read &&
//         leaf3.header.size == 170;

//     std::cout
//         << "\n========== LEAF SIZES ==========\n";

//     std::cout
//         << "Leaf 1 size: "
//         << (leaf1SizeCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "Leaf 2 size: "
//         << (leaf2SizeCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "Leaf 3 size: "
//         << (leaf3SizeCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Leaf boundaries
//     // ==========================================

//     bool boundary1Correct =
//         leaf1Read &&
//         leaf2Read &&
//         leaf1.entries[
//             leaf1.header.size - 1
//         ].key == 170 &&
//         leaf2.entries[0].key == 171;

//     bool boundary2Correct =
//         leaf2Read &&
//         leaf3Read &&
//         leaf2.entries[
//             leaf2.header.size - 1
//         ].key == 340 &&
//         leaf3.entries[0].key == 341;

//     std::cout
//         << "\n========== LEAF BOUNDARIES ==========\n";

//     std::cout
//         << "170 -> 171 boundary: "
//         << (boundary1Correct
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "340 -> 341 boundary: "
//         << (boundary2Correct
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Leaf chain
//     // ==========================================

//     bool chainCorrect =
//         leaf1Read &&
//         leaf2Read &&
//         leaf3Read &&
//         leaf1.nextPageId == leaf2PageId &&
//         leaf2.nextPageId == leaf3PageId &&
//         leaf3.nextPageId == -1;

//     std::cout
//         << "\nLeaf chain: "
//         << (chainCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Check sorted ordering inside leaves
//     // ==========================================

//     bool orderingCorrect = true;

//     for (int i = 1;
//          i < leaf1.header.size;
//          i++)
//     {
//         if (leaf1.entries[i - 1].key >=
//             leaf1.entries[i].key)
//         {
//             orderingCorrect = false;
//             break;
//         }
//     }

//     for (int i = 1;
//          i < leaf2.header.size;
//          i++)
//     {
//         if (leaf2.entries[i - 1].key >=
//             leaf2.entries[i].key)
//         {
//             orderingCorrect = false;
//             break;
//         }
//     }

//     for (int i = 1;
//          i < leaf3.header.size;
//          i++)
//     {
//         if (leaf3.entries[i - 1].key >=
//             leaf3.entries[i].key)
//         {
//             orderingCorrect = false;
//             break;
//         }
//     }

//     std::cout
//         << "Leaf ordering: "
//         << (orderingCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Check ALL keys
//     // ==========================================

//     bool allKeysPresent = true;

//     for (int key = 1;
//          key <= totalKeys;
//          key++)
//     {
//         RecordPointer result =
//             tree.search(key);

//         if (result.pageId != key ||
//             result.slotId != key)
//         {
//             std::cout
//                 << "Search failed for key: "
//                 << key
//                 << '\n';

//             allKeysPresent = false;
//             break;
//         }
//     }

//     std::cout
//         << "\nAll keys searchable: "
//         << (allKeysPresent
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Boundary searches
//     // ==========================================

//     int testKeys[] =
//     {
//         1,
//         170,
//         171,
//         200,
//         340,
//         341,
//         400,
//         510
//     };

//     bool boundarySearchCorrect = true;

//     for (int key : testKeys)
//     {
//         RecordPointer result =
//             tree.search(key);

//         if (result.pageId != key ||
//             result.slotId != key)
//         {
//             boundarySearchCorrect = false;

//             std::cout
//                 << "Boundary search failed: "
//                 << key
//                 << '\n';
//         }
//     }

//     std::cout
//         << "Boundary searches: "
//         << (boundarySearchCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Print final tree
//     // ==========================================

//     std::cout
//         << "\n========== TREE STRUCTURE ==========\n\n";

//     std::cout
//         << "                    Root: "
//         << rootPageId
//         << '\n';

//     std::cout
//         << "                 ["
//         << root.entries[0].key
//         << " | "
//         << root.entries[1].key
//         << "]\n";

//     std::cout
//         << "                /     |     \\\n";

//     std::cout
//         << "               /      |      \\\n";

//     std::cout
//         << "          Leaf: "
//         << leaf1PageId
//         << "  Leaf: "
//         << leaf2PageId
//         << "  Leaf: "
//         << leaf3PageId
//         << '\n';

//     std::cout
//         << "         [1..170] [171..340] [341..510]\n";

//     // ==========================================
//     // Final result
//     // ==========================================

//     bool allPassed =
//         insertSuccess &&
//         rootRead &&
//         rootSizeCorrect &&
//         firstSeparatorCorrect &&
//         secondSeparatorCorrect &&
//         firstChildValid &&
//         secondChildValid &&
//         thirdChildValid &&
//         leaf1Read &&
//         leaf2Read &&
//         leaf3Read &&
//         leaf1SizeCorrect &&
//         leaf2SizeCorrect &&
//         leaf3SizeCorrect &&
//         boundary1Correct &&
//         boundary2Correct &&
//         chainCorrect &&
//         orderingCorrect &&
//         allKeysPresent &&
//         boundarySearchCorrect;

//     std::cout
//         << "\n========================================\n";

//     if (allPassed)
//     {
//         std::cout
//             << "DAY 13: MULTIPLE LEAF SPLITS PASS\n";
//     }
//     else
//     {
//         std::cout
//             << "DAY 13: MULTIPLE LEAF SPLITS FAIL\n";
//     }

//     std::cout
//         << "========================================\n";

//     // ==========================================
//     // Disk flush
//     // ==========================================

//     bpm.FlushAllPages();

//     std::cout
//         << "\nDisk flush completed.\n";

//     return allPassed ? 0 : 1;
// }

// #include <iostream>
// #include <cstdio>

// #include "buffer/buffer_pool_manager.h"
// #include "bptree/b_plus_tree.h"
// #include "bptree/b_plus_tree_internal_page.h"
// #include "bptree/b_plus_tree_leaf_page.h"
// #include "storage/record_pointer.h"

// int main()
// {
//     const std::string filename =
//         "../data/day14_test.idx";

//     // ==========================================
//     // Remove old test file
//     // ==========================================

//     std::remove(filename.c_str());

//     std::cout
//         << "\n========================================\n"
//         << "DAY 14: PARENT POINTER TEST\n"
//         << "========================================\n\n";

//     // ==========================================
//     // Buffer Pool
//     // ==========================================

//     BufferPoolManager bpm(10);

//     // ==========================================
//     // B+ Tree
//     // ==========================================

//     BPlusTree tree(
//         &bpm,
//         filename);

//     if (!tree.initialize())
//     {
//         std::cout
//             << "Tree initialization: FAIL\n";

//         return 1;
//     }

//     std::cout
//         << "Tree initialization: PASS\n";

//     // ==========================================
//     // DAY 13 BASE TREE
//     //
//     // Expected:
//     //
//     //              Root
//     //           [171 | 341]
//     //          /     |     \
//     //       L1      L2      L3
//     //
//     //       1..170
//     //       171..340
//     //       341..510
//     // ==========================================

//     bool insertSuccess = true;

//     for (int key = 1;
//          key <= 510;
//          key++)
//     {
//         RecordPointer pointer;

//         pointer.pageId = key;
//         pointer.slotId = key;

//         if (!tree.insert(
//                 key,
//                 pointer))
//         {
//             std::cout
//                 << "Insert failed for key: "
//                 << key
//                 << '\n';

//             insertSuccess = false;
//             break;
//         }
//     }

//     std::cout
//         << "\nInsert 1..510: "
//         << (insertSuccess
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     if (!insertSuccess)
//     {
//         return 1;
//     }

//     // ==========================================
//     // Get root
//     // ==========================================

//     int rootPageId =
//         tree.getRootPageId();

//     std::cout
//         << "Root Page ID: "
//         << rootPageId
//         << '\n';

//     // ==========================================
//     // Read root
//     // ==========================================

//     InternalPage root{};

//     bool rootRead =
//         tree.readInternalPage(
//             rootPageId,
//             root);

//     std::cout
//         << "Root read: "
//         << (rootRead
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     if (!rootRead)
//     {
//         return 1;
//     }

//     // ==========================================
//     // Root parent should be -1
//     // ==========================================

//     bool rootParentCorrect =
//         root.header.parentPageId == -1;

//     std::cout
//         << "\nRoot parent (-1): "
//         << (rootParentCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Root information
//     // ==========================================

//     std::cout
//         << "\n========== INITIAL ROOT ==========\n";

//     std::cout
//         << "Root ID: "
//         << rootPageId
//         << '\n';

//     std::cout
//         << "Root parent: "
//         << root.header.parentPageId
//         << '\n';

//     std::cout
//         << "Root size: "
//         << root.header.size
//         << '\n';

//     std::cout
//         << "First child: "
//         << root.firstChildPageId
//         << '\n';

//     for (int i = 0;
//          i < root.header.size;
//          i++)
//     {
//         std::cout
//             << "Entry "
//             << i
//             << ": key="
//             << root.entries[i].key
//             << ", child="
//             << root.entries[i].childPageId
//             << '\n';
//     }

//     // ==========================================
//     // Get first three leaves
//     // ==========================================

//     int leaf1PageId =
//         root.firstChildPageId;

//     int leaf2PageId =
//         root.entries[0].childPageId;

//     int leaf3PageId =
//         root.entries[1].childPageId;

//     LeafPage leaf1{};
//     LeafPage leaf2{};
//     LeafPage leaf3{};

//     bool leaf1Read =
//         tree.readLeafPage(
//             leaf1PageId,
//             leaf1);

//     bool leaf2Read =
//         tree.readLeafPage(
//             leaf2PageId,
//             leaf2);

//     bool leaf3Read =
//         tree.readLeafPage(
//             leaf3PageId,
//             leaf3);

//     // ==========================================
//     // Verify initial parent pointers
//     // ==========================================

//     bool leaf1ParentCorrect =
//         leaf1Read &&
//         leaf1.header.parentPageId ==
//             rootPageId;

//     bool leaf2ParentCorrect =
//         leaf2Read &&
//         leaf2.header.parentPageId ==
//             rootPageId;

//     bool leaf3ParentCorrect =
//         leaf3Read &&
//         leaf3.header.parentPageId ==
//             rootPageId;

//     std::cout
//         << "\n========== INITIAL PARENT POINTERS ==========\n";

//     std::cout
//         << "Leaf 1 parent: "
//         << (leaf1ParentCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << " ("
//         << leaf1.header.parentPageId
//         << ")\n";

//     std::cout
//         << "Leaf 2 parent: "
//         << (leaf2ParentCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << " ("
//         << leaf2.header.parentPageId
//         << ")\n";

//     std::cout
//         << "Leaf 3 parent: "
//         << (leaf3ParentCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << " ("
//         << leaf3.header.parentPageId
//         << ")\n";

//     // ==========================================
//     // DAY 14:
//     //
//     // Insert 511..680
//     //
//     // This fills Leaf 3:
//     //
//     // Leaf 3 currently = 341..510
//     //
//     // 511..680 = 170 new entries
//     //
//     // When Leaf 3 reaches 341 entries,
//     // it splits.
//     //
//     // Expected:
//     //
//     //              Root
//     //       [171 | 341 | 511]
//     //       /     |     |     \
//     //      L1     L2    L3      L4
//     //
//     // 1..170
//     // 171..340
//     // 341..510
//     // 511..680
//     // ==========================================

//     bool additionalInsertSuccess = true;

//     for (int key = 511;
//          key <= 680;
//          key++)
//     {
//         RecordPointer pointer;

//         pointer.pageId = key;
//         pointer.slotId = key;

//         if (!tree.insert(
//                 key,
//                 pointer))
//         {
//             std::cout
//                 << "Additional insert failed for key: "
//                 << key
//                 << '\n';

//             additionalInsertSuccess = false;
//             break;
//         }
//     }

//     std::cout
//         << "\nInsert 511..680: "
//         << (additionalInsertSuccess
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     if (!additionalInsertSuccess)
//     {
//         return 1;
//     }

//     // ==========================================
//     // Root should remain same
//     // ==========================================

//     int newRootPageId =
//         tree.getRootPageId();

//     bool rootUnchanged =
//         newRootPageId == rootPageId;

//     std::cout
//         << "Root unchanged: "
//         << (rootUnchanged
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Read updated root
//     // ==========================================

//     InternalPage newRoot{};

//     bool newRootRead =
//         tree.readInternalPage(
//             newRootPageId,
//             newRoot);

//     std::cout
//         << "Updated root read: "
//         << (newRootRead
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     if (!newRootRead)
//     {
//         return 1;
//     }

//     // ==========================================
//     // Root validation
//     // ==========================================

//     bool rootSizeCorrect =
//         newRoot.header.size == 3;

//     bool separator1Correct =
//         newRoot.entries[0].key == 171;

//     bool separator2Correct =
//         newRoot.entries[1].key == 341;

//     bool separator3Correct =
//         newRoot.entries[2].key == 511;

//     bool rootParentStillCorrect =
//         newRoot.header.parentPageId == -1;

//     std::cout
//         << "\n========== UPDATED ROOT ==========\n";

//     std::cout
//         << "Root size = 3: "
//         << (rootSizeCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "Separator 171: "
//         << (separator1Correct
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "Separator 341: "
//         << (separator2Correct
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "Separator 511: "
//         << (separator3Correct
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "Root parent (-1): "
//         << (rootParentStillCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Get fourth leaf
//     // ==========================================

//     int leaf4PageId =
//         newRoot.entries[2].childPageId;

//     LeafPage leaf4{};

//     bool leaf4Read =
//         tree.readLeafPage(
//             leaf4PageId,
//             leaf4);

//     std::cout
//         << "\nFourth leaf read: "
//         << (leaf4Read
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Re-read leaves
//     //
//     // Leaf 3 may have been modified
//     // during the split.
//     // ==========================================

//     LeafPage updatedLeaf1{};
//     LeafPage updatedLeaf2{};
//     LeafPage updatedLeaf3{};

//     tree.readLeafPage(
//         leaf1PageId,
//         updatedLeaf1);

//     tree.readLeafPage(
//         leaf2PageId,
//         updatedLeaf2);

//     tree.readLeafPage(
//         leaf3PageId,
//         updatedLeaf3);

//     // ==========================================
//     // Parent pointer validation
//     // ==========================================

//     bool updatedLeaf1Parent =
//         updatedLeaf1.header.parentPageId ==
//         newRootPageId;

//     bool updatedLeaf2Parent =
//         updatedLeaf2.header.parentPageId ==
//         newRootPageId;

//     bool updatedLeaf3Parent =
//         updatedLeaf3.header.parentPageId ==
//         newRootPageId;

//     bool leaf4Parent =
//         leaf4Read &&
//         leaf4.header.parentPageId ==
//             newRootPageId;

//     std::cout
//         << "\n========== FINAL PARENT POINTERS ==========\n";

//     std::cout
//         << "Leaf 1 parent: "
//         << (updatedLeaf1Parent
//                 ? "PASS"
//                 : "FAIL")
//         << " ("
//         << updatedLeaf1.header.parentPageId
//         << ")\n";

//     std::cout
//         << "Leaf 2 parent: "
//         << (updatedLeaf2Parent
//                 ? "PASS"
//                 : "FAIL")
//         << " ("
//         << updatedLeaf2.header.parentPageId
//         << ")\n";

//     std::cout
//         << "Leaf 3 parent: "
//         << (updatedLeaf3Parent
//                 ? "PASS"
//                 : "FAIL")
//         << " ("
//         << updatedLeaf3.header.parentPageId
//         << ")\n";

//     std::cout
//         << "Leaf 4 parent: "
//         << (leaf4Parent
//                 ? "PASS"
//                 : "FAIL")
//         << " ("
//         << leaf4.header.parentPageId
//         << ")\n";

//     // ==========================================
//     // Leaf sizes
//     // ==========================================

//     bool leaf1SizeCorrect =
//         updatedLeaf1.header.size == 170;

//     bool leaf2SizeCorrect =
//         updatedLeaf2.header.size == 170;

//     bool leaf3SizeCorrect =
//         updatedLeaf3.header.size == 170;

//     bool leaf4SizeCorrect =
//         leaf4Read &&
//         leaf4.header.size == 170;

//     std::cout
//         << "\n========== LEAF SIZES ==========\n";

//     std::cout
//         << "Leaf 1 size: "
//         << (leaf1SizeCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "Leaf 2 size: "
//         << (leaf2SizeCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "Leaf 3 size: "
//         << (leaf3SizeCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "Leaf 4 size: "
//         << (leaf4SizeCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Boundary validation
//     // ==========================================

//     bool boundary1 =
//         updatedLeaf1.entries[updatedLeaf1.header.size - 1].key == 170 &&
//         updatedLeaf2.entries[0].key == 171;

//     bool boundary2 =
//         updatedLeaf2.entries[updatedLeaf2.header.size - 1].key == 340 &&
//         updatedLeaf3.entries[0].key == 341;

//     bool boundary3 =
//         updatedLeaf3.entries[updatedLeaf3.header.size - 1].key == 510 &&
//         leaf4.entries[0].key == 511;

//     bool boundary4 =
//         leaf4.entries[leaf4.header.size - 1].key == 680;

//     std::cout
//         << "\n========== LEAF BOUNDARIES ==========\n";

//     std::cout
//         << "170 -> 171: "
//         << (boundary1
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "340 -> 341: "
//         << (boundary2
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "510 -> 511: "
//         << (boundary3
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "680 final key: "
//         << (boundary4
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Leaf chain
//     // ==========================================

//     bool chainCorrect =
//         updatedLeaf1.nextPageId == leaf2PageId &&
//         updatedLeaf2.nextPageId == leaf3PageId &&
//         updatedLeaf3.nextPageId == leaf4PageId &&
//         leaf4.nextPageId == -1;

//     std::cout
//         << "\nLeaf chain: "
//         << (chainCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Search validation
//     // ==========================================

//     bool allKeysSearchable = true;

//     for (int key = 1;
//          key <= 680;
//          key++)
//     {
//         RecordPointer result =
//             tree.search(key);

//         if (result.pageId != key ||
//             result.slotId != key)
//         {
//             std::cout
//                 << "Search failed for key: "
//                 << key
//                 << '\n';

//             allKeysSearchable = false;
//             break;
//         }
//     }

//     std::cout
//         << "All keys searchable: "
//         << (allKeysSearchable
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Print final structure
//     // ==========================================

//     std::cout
//         << "\n========== FINAL TREE STRUCTURE ==========\n\n";

//     std::cout
//         << "                    Root: "
//         << newRootPageId
//         << '\n';

//     std::cout
//         << "                 ["
//         << newRoot.entries[0].key
//         << " | "
//         << newRoot.entries[1].key
//         << " | "
//         << newRoot.entries[2].key
//         << "]\n";

//     std::cout
//         << "                /     |     |     \\\n";

//     std::cout
//         << "               /      |     |      \\\n";

//     std::cout
//         << "          Leaf: "
//         << leaf1PageId
//         << "  Leaf: "
//         << leaf2PageId
//         << "  Leaf: "
//         << leaf3PageId
//         << "  Leaf: "
//         << leaf4PageId
//         << '\n';

//     std::cout
//         << "         [1..170] [171..340] "
//         << "[341..510] [511..680]\n";

//     // ==========================================
//     // Final result
//     // ==========================================

//     std::cout
//         << "LEAF_MAX_ENTRIES = "
//         << LEAF_MAX_ENTRIES
//         << '\n';

//     std::cout
//         << "INTERNAL_MAX_ENTRIES = "
//         << INTERNAL_MAX_ENTRIES
//         << '\n';

//     bool allPassed =
//         insertSuccess &&
//         rootRead &&
//         rootParentCorrect &&
//         leaf1ParentCorrect &&
//         leaf2ParentCorrect &&
//         leaf3ParentCorrect &&
//         additionalInsertSuccess &&
//         rootUnchanged &&
//         newRootRead &&
//         rootSizeCorrect &&
//         separator1Correct &&
//         separator2Correct &&
//         separator3Correct &&
//         rootParentStillCorrect &&
//         leaf4Read &&
//         updatedLeaf1Parent &&
//         updatedLeaf2Parent &&
//         updatedLeaf3Parent &&
//         leaf4Parent &&
//         leaf1SizeCorrect &&
//         leaf2SizeCorrect &&
//         leaf3SizeCorrect &&
//         leaf4SizeCorrect &&
//         boundary1 &&
//         boundary2 &&
//         boundary3 &&
//         boundary4 &&
//         chainCorrect &&
//         allKeysSearchable;

//     std::cout
//         << "\n========================================\n";

//     if (allPassed)
//     {
//         std::cout
//             << "DAY 14: PARENT POINTER TEST PASS\n";
//     }
//     else
//     {
//         std::cout
//             << "DAY 14: PARENT POINTER TEST FAIL\n";
//     }

//     std::cout
//         << "========================================\n";

//     // ==========================================
//     // Flush
//     // ==========================================

//     bpm.FlushAllPages();

//     std::cout
//         << "\nDisk flush completed.\n";

//     return allPassed ? 0 : 1;
// }

// #include <iostream>
// #include <cstdio>

// #include "buffer/buffer_pool_manager.h"
// #include "bptree/b_plus_tree.h"
// #include "bptree/b_plus_tree_internal_page.h"
// #include "bptree/b_plus_tree_leaf_page.h"
// #include "storage/record_pointer.h"

// int main()
// {
//     const std::string filename =
//         "../data/day15_test.idx";

//     std::remove(filename.c_str());

//     std::cout
//         << "\n========================================\n"
//         << "DAY 15: INTERNAL NODE SPLIT TEST\n"
//         << "========================================\n\n";

//     std::cout
//         << "LEAF_MAX_ENTRIES = "
//         << LEAF_MAX_ENTRIES
//         << '\n';

//     std::cout
//         << "INTERNAL_MAX_ENTRIES = "
//         << INTERNAL_MAX_ENTRIES
//         << '\n';

//     // ==========================================
//     // Buffer Pool
//     // ==========================================

//     BufferPoolManager bpm(20);

//     // ==========================================
//     // B+ Tree
//     // ==========================================

//     BPlusTree tree(
//         &bpm,
//         filename);

//     if (!tree.initialize())
//     {
//         std::cout
//             << "Tree initialization: FAIL\n";

//         return 1;
//     }

//     std::cout
//         << "Tree initialization: PASS\n";

//     // ==========================================
//     // Number of leaves required to fill root
//     //
//     // Internal node with 509 entries can have
//     // 510 children.
//     //
//     // We need one additional child to overflow.
//     // ==========================================

//     const int requiredLeaves =
//         INTERNAL_MAX_ENTRIES + 1;

//     const int keysBeforeRootOverflow =
//         requiredLeaves * LEAF_MAX_ENTRIES;

//     const int totalKeys =
//         keysBeforeRootOverflow + 1;

//     std::cout
//         << "\nRequired leaves: "
//         << requiredLeaves
//         << '\n';

//     std::cout
//         << "Keys before root overflow: "
//         << keysBeforeRootOverflow
//         << '\n';

//     std::cout
//         << "Total test keys: "
//         << totalKeys
//         << '\n';

//     // ==========================================
//     // Insert all keys
//     // ==========================================

//     bool insertSuccess = true;

//     for (int key = 1;
//          key <= totalKeys;
//          key++)
//     {
//         RecordPointer pointer;

//         pointer.pageId = key;
//         pointer.slotId = key;

//         if (!tree.insert(
//                 key,
//                 pointer))
//         {
//             std::cout
//                 << "\nInsert failed at key: "
//                 << key
//                 << '\n';

//             insertSuccess = false;
//             break;
//         }

//         // Progress output
//         if (key % 10000 == 0)
//         {
//             std::cout
//                 << "Inserted "
//                 << key
//                 << " keys...\n";
//         }
//     }

//     std::cout
//         << "\nLarge insertion: "
//         << (insertSuccess
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     if (!insertSuccess)
//     {
//         bpm.FlushAllPages();
//         return 1;
//     }

//     // ==========================================
//     // Read root
//     // ==========================================

//     int rootPageId =
//         tree.getRootPageId();

//     std::cout
//         << "\nRoot Page ID: "
//         << rootPageId
//         << '\n';

//     InternalPage root{};

//     bool rootRead =
//         tree.readInternalPage(
//             rootPageId,
//             root);

//     std::cout
//         << "Root read: "
//         << (rootRead
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     if (!rootRead)
//     {
//         return 1;
//     }

//     // ==========================================
//     // Root should now be a NEW root.
//     //
//     // Its children should be internal pages.
//     // ==========================================

//     bool rootIsInternal =
//         root.header.type ==
//         BPlusPageType::INTERNAL;

//     bool rootParentCorrect =
//         root.header.parentPageId == -1;

//     std::cout
//         << "\n========== ROOT VALIDATION ==========\n";

//     std::cout
//         << "Root is INTERNAL: "
//         << (rootIsInternal
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "Root parent = -1: "
//         << (rootParentCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "Root size: "
//         << root.header.size
//         << '\n';

//     std::cout
//         << "Root first child: "
//         << root.firstChildPageId
//         << '\n';

//     for (int i = 0;
//          i < root.header.size;
//          i++)
//     {
//         std::cout
//             << "Root entry "
//             << i
//             << ": key="
//             << root.entries[i].key
//             << ", child="
//             << root.entries[i].childPageId
//             << '\n';
//     }

//     // ==========================================
//     // Root should have exactly 2 children
//     // after splitting.
//     //
//     // Therefore:
//     //
//     // root.header.size == 1
//     //
//     // firstChild -> left internal
//     // entries[0] -> right internal
//     // ==========================================

//     bool rootSizeCorrect =
//         root.header.size == 2;

//     bool rootHasThreeChildren =
//     root.firstChildPageId != -1 &&
//     root.entries[0].childPageId != -1 &&
//     root.entries[1].childPageId != -1;

//     std::cout
//         << "\nRoot size = 2: "
//         << (rootSizeCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "Root has two children: "
//         << (rootHasThreeChildren
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Read left internal node
//     // ==========================================

//     int leftInternalPageId =
//         root.firstChildPageId;

//     int rightInternalPageId =
//         root.entries[0].childPageId;

//     InternalPage leftInternal{};
//     InternalPage rightInternal{};

//     bool leftRead =
//         tree.readInternalPage(
//             leftInternalPageId,
//             leftInternal);

//     bool rightRead =
//         tree.readInternalPage(
//             rightInternalPageId,
//             rightInternal);

//     std::cout
//         << "\nLeft internal read: "
//         << (leftRead
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "Right internal read: "
//         << (rightRead
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     if (!leftRead ||
//         !rightRead)
//     {
//         return 1;
//     }

//     // ==========================================
//     // Parent pointers
//     // ==========================================

//     bool leftParentCorrect =
//         leftInternal.header.parentPageId ==
//         rootPageId;

//     bool rightParentCorrect =
//         rightInternal.header.parentPageId ==
//         rootPageId;

//     std::cout
//         << "\n========== INTERNAL PARENT POINTERS ==========\n";

//     std::cout
//         << "Left internal parent: "
//         << (leftParentCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << " ("
//         << leftInternal.header.parentPageId
//         << ")\n";

//     std::cout
//         << "Right internal parent: "
//         << (rightParentCorrect
//                 ? "PASS"
//                 : "FAIL")
//         << " ("
//         << rightInternal.header.parentPageId
//         << ")\n";

//     // ==========================================
//     // Internal sizes
//     // ==========================================

//     std::cout
//         << "\n========== INTERNAL SIZES ==========\n";

//     std::cout
//         << "Left internal size: "
//         << leftInternal.header.size
//         << '\n';

//     std::cout
//         << "Right internal size: "
//         << rightInternal.header.size
//         << '\n';

//     bool leftNotFull =
//         leftInternal.header.size <
//         INTERNAL_MAX_ENTRIES;

//     bool rightNotFull =
//         rightInternal.header.size <
//         INTERNAL_MAX_ENTRIES;

//     std::cout
//         << "Left internal valid size: "
//         << (leftNotFull
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     std::cout
//         << "Right internal valid size: "
//         << (rightNotFull
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Separator validation
//     // ==========================================

//     int rootSeparator =
//         root.entries[0].key;

//     std::cout
//         << "\nRoot separator: "
//         << rootSeparator
//         << '\n';

//     bool separatorValid =
//         rootSeparator > 0;

//     if (leftInternal.header.size > 0 &&
//         rightInternal.header.size > 0)
//     {
//         int leftLastKey =
//             leftInternal.entries[
//                 leftInternal.header.size - 1
//             ].key;

//         int rightFirstKey =
//             rightInternal.entries[0].key;

//         separatorValid =
//             separatorValid &&
//             leftLastKey < rootSeparator &&
//             rightFirstKey >= rootSeparator;

//         std::cout
//             << "Left last separator: "
//             << leftLastKey
//             << '\n';

//         std::cout
//             << "Right first separator: "
//             << rightFirstKey
//             << '\n';
//     }

//     std::cout
//         << "Separator ordering: "
//         << (separatorValid
//                 ? "PASS"
//                 : "FAIL")
//         << '\n';

//     // ==========================================
//     // Search validation
//     //
//     // Test important boundary keys instead
//     // of reading all 172k results.
//     // ==========================================

//     int testKeys[] =
//     {
//         1,
//         339,
//         340,
//         1000,
//         50000,
//         100000,
//         150000,
//         keysBeforeRootOverflow,
//         totalKeys
//     };

//     bool searchCorrect = true;

//     std::cout
//         << "\n========== SEARCH VALIDATION ==========\n";

//     for (int key : testKeys)
//     {
//         RecordPointer result =
//             tree.search(key);

//         bool correct =
//             result.pageId == key &&
//             result.slotId == key;

//         std::cout
//             << "Search "
//             << key
//             << ": "
//             << (correct
//                     ? "PASS"
//                     : "FAIL")
//             << '\n';

//         if (!correct)
//         {
//             searchCorrect = false;
//         }
//     }

//     // ==========================================
//     // Final result
//     // ==========================================

//     bool allPassed =
//         insertSuccess &&
//         rootRead &&
//         rootIsInternal &&
//         rootParentCorrect &&
//         rootSizeCorrect &&
//         rootHasThreeChildren &&
//         leftRead &&
//         rightRead &&
//         leftParentCorrect &&
//         rightParentCorrect &&
//         leftNotFull &&
//         rightNotFull &&
//         separatorValid &&
//         searchCorrect;

//     std::cout
//         << "\n========================================\n";

//     if (allPassed)
//     {
//         std::cout
//             << "DAY 15: INTERNAL NODE SPLIT PASS\n";
//     }
//     else
//     {
//         std::cout
//             << "DAY 15: INTERNAL NODE SPLIT FAIL\n";
//     }

//     std::cout
//         << "========================================\n";

//     // ==========================================
//     // Flush
//     // ==========================================

//     bpm.FlushAllPages();

//     std::cout
//         << "\nDisk flush completed.\n";


//     return allPassed ? 0 : 1;
// }


// #include <iostream>
// #include <cstdio>
// #include <vector>

// #include "bptree/b_plus_tree.h"
// #include "bptree/b_plus_tree_page_manager.h"
// #include "buffer/buffer_pool_manager.h"

// int main()
// {
//     const std::string filename =
//         "../data/day16_test.idx";

//     // std::remove(filename.c_str());

//     // =========================================================
//     // PHASE 1: BUILD TREE
//     // =========================================================

//     std::cout
//         << "\n========== PHASE 1: BUILD TREE ==========\n";

//     int expectedRootPageId = -1;

//     {
//         BufferPoolManager bufferPool(20);

//         BPlusTree tree(
//             &bufferPool,
//             filename);

//         if (!tree.initialize())
//         {
//             std::cout
//                 << "Tree initialization: FAIL\n";

//             return 1;
//         }

//         // Insert enough keys to create
//         // multiple internal pages.
//         for (int key = 1;
//              key <= 100000;
//              key++)
//         {
//             RecordPointer pointer{
//                 1,
//                 key};

//             if (!tree.insert(
//                     key,
//                     pointer))
//             {
//                 std::cout
//                     << "Insert failed at key: "
//                     << key
//                     << '\n';

//                 return 1;
//             }
//         }

//         BPlusTreeMetadata metadata{};

//         BPlusTreePageManager manager(
//             bufferPool,
//             filename);

//         if (!manager.readMetadata(metadata))
//         {
//             std::cout
//                 << "Metadata read: FAIL\n";

//             return 1;
//         }

//         expectedRootPageId =
//             metadata.rootPageId;

//         std::cout
//             << "Root before restart: "
//             << expectedRootPageId
//             << '\n';

//         std::cout
//             << "Tree construction: PASS\n";

//         bufferPool.FlushAllPages();

//         std::cout
//             << "Disk flush: PASS\n";
//     }

//     // =========================================================
//     // PHASE 2: REOPEN TREE
//     // =========================================================

//     std::cout
//         << "\n========== PHASE 2: REOPEN TREE ==========\n";

//     {
//         BufferPoolManager bufferPool(20);

//         BPlusTreePageManager manager(
//             bufferPool,
//             filename);

//         if (!manager.initializeIndex())
//         {
//             std::cout
//                 << "Index initialization: FAIL\n";

//             return 1;
//         }

//         BPlusTreeMetadata metadata{};

//         if (!manager.readMetadata(metadata))
//         {
//             std::cout
//                 << "Metadata recovery: FAIL\n";

//             return 1;
//         }

//         std::cout
//             << "Recovered Root: "
//             << metadata.rootPageId
//             << '\n';

//         bool rootRecovered =
//             metadata.rootPageId ==
//             expectedRootPageId;

//         std::cout
//             << "Root recovery: "
//             << (rootRecovered
//                     ? "PASS"
//                     : "FAIL")
//             << '\n';

//         if (!rootRecovered)
//             return 1;

//         // =====================================================
//         // READ ROOT
//         // =====================================================

//         InternalPage root{};

//         bool rootRead =
//             manager.readInternalPage(
//                 metadata.rootPageId,
//                 root);

//         std::cout
//             << "Root read: "
//             << (rootRead
//                     ? "PASS"
//                     : "FAIL")
//             << '\n';

//         if (!rootRead)
//             return 1;

//         std::cout
//             << "\n========== RECOVERED ROOT ==========\n";

//         std::cout
//             << "Root Page ID: "
//             << root.header.pageId
//             << '\n';

//         std::cout
//             << "Root Parent: "
//             << root.header.parentPageId
//             << '\n';

//         std::cout
//             << "Root Size: "
//             << root.header.size
//             << '\n';

//         std::cout
//             << "First Child: "
//             << root.firstChildPageId
//             << '\n';

//         for (int i = 0;
//              i < root.header.size;
//              i++)
//         {
//             std::cout
//                 << "Entry "
//                 << i
//                 << ": key="
//                 << root.entries[i].key
//                 << " child="
//                 << root.entries[i].childPageId
//                 << '\n';
//         }

//         // =====================================================
//         // BASIC ROOT VALIDATION
//         // =====================================================

//         bool rootIsValid =
//             root.header.type ==
//                 BPlusPageType::INTERNAL &&
//             root.header.parentPageId == -1 &&
//             root.firstChildPageId != -1 &&
//             root.header.size > 0;

//         std::cout
//             << "\nRoot structure: "
//             << (rootIsValid
//                     ? "PASS"
//                     : "FAIL")
//             << '\n';

//         // =====================================================
//         // VERIFY CHILD PAGES EXIST
//         // =====================================================

//         bool childrenValid = true;

//         for (int i = -1;
//              i < root.header.size;
//              i++)
//         {
//             int childPageId;

//             if (i == -1)
//             {
//                 childPageId =
//                     root.firstChildPageId;
//             }
//             else
//             {
//                 childPageId =
//                     root.entries[i].childPageId;
//             }

//             BPlusTreePageHeader header{};

//             Page* page =
//                 bufferPool.FetchPage(
//                     filename,
//                     childPageId);

//             if (page == nullptr)
//             {
//                 childrenValid = false;
//                 break;
//             }

//             deserializePageHeader(
//                 *page,
//                 header);

//             bufferPool.UnpinPage(
//                 filename,
//                 childPageId);

//             if (header.pageId !=
//                 childPageId)
//             {
//                 childrenValid = false;
//                 break;
//             }
//         }

//         std::cout
//             << "Root children recovered: "
//             << (childrenValid
//                     ? "PASS"
//                     : "FAIL")
//             << '\n';

//         // =====================================================
//         // FINAL RESULT
//         // =====================================================

//         bool passed =
//             rootRecovered &&
//             rootRead &&
//             rootIsValid &&
//             childrenValid;

//         std::cout
//             << "\n========================================\n";

//         std::cout
//             << "DAY 16: PERSISTENCE TEST "
//             << (passed
//                     ? "PASS"
//                     : "FAIL")
//             << '\n';

//         std::cout
//             << "========================================\n";

//         bufferPool.FlushAllPages();

//         if (!passed)
//             return 1;
//     }

//     return 0;
// }


#include <iostream>

#include "bptree/b_plus_tree.h"
#include "bptree/b_plus_tree_page_manager.h"
#include "buffer/buffer_pool_manager.h"

int main()
{
    const std::string filename =
        "../data/day16_test.idx";

    // IMPORTANT:
    // Do NOT delete the file.
    // Day 17 tests persistence across program executions.

    // =========================================================
    // OPEN EXISTING INDEX
    // =========================================================

    BufferPoolManager bufferPool(20);

    BPlusTree tree(
        &bufferPool,
        filename);

    if (!tree.initialize())
    {
        std::cout
            << "Tree initialization: FAIL\n";

        return 1;
    }

    std::cout
        << "\n========== DAY 17: PERSISTENT SEARCH ==========\n";

    // =========================================================
    // READ METADATA
    // =========================================================

    BPlusTreePageManager manager(
        bufferPool,
        filename);

    BPlusTreeMetadata metadata{};

    if (!manager.readMetadata(metadata))
    {
        std::cout
            << "Metadata read: FAIL\n";

        return 1;
    }

    std::cout
        << "Recovered Root: "
        << metadata.rootPageId
        << '\n';

    bool metadataValid =
        metadata.rootPageId != -1;

    std::cout
        << "Metadata recovery: "
        << (metadataValid
                ? "PASS"
                : "FAIL")
        << '\n';

    if (!metadataValid)
        return 1;

    // =========================================================
    // SEARCH TEST
    // =========================================================

    int testKeys[] =
    {
        1,
        339,
        340,
        341,
        1000,
        50000,
        100000
    };

    bool allSearchesPassed = true;

    std::cout
        << "\n========== SEARCH VALIDATION ==========\n";

    for (int key : testKeys)
    {
        RecordPointer result =
            tree.search(key);

        bool found =
            result.pageId != -1 &&
            result.slotId != -1;

        bool pointerCorrect =
            found &&
            result.slotId == key;

        bool passed =
            found &&
            pointerCorrect;

        std::cout
            << "Search "
            << key
            << ": "
            << (passed
                    ? "PASS"
                    : "FAIL")
            << '\n';

        if (!passed)
        {
            std::cout
                << "  Returned pageId = "
                << result.pageId
                << '\n';

            std::cout
                << "  Returned slotId = "
                << result.slotId
                << '\n';

            allSearchesPassed = false;
        }
    }

    // =========================================================
    // MISSING KEY
    // =========================================================

    RecordPointer missing =
        tree.search(100001);

    bool missingKeyPassed =
        missing.pageId == -1 &&
        missing.slotId == -1;

    std::cout
        << "\nSearch missing key 100001: "
        << (missingKeyPassed
                ? "PASS"
                : "FAIL")
        << '\n';

    if (!missingKeyPassed)
    {
        std::cout
            << "Returned pageId = "
            << missing.pageId
            << '\n';

        std::cout
            << "Returned slotId = "
            << missing.slotId
            << '\n';
    }

    // =========================================================
    // FINAL RESULT
    // =========================================================

    bool passed =
        metadataValid &&
        allSearchesPassed &&
        missingKeyPassed;

    std::cout
        << "\n========================================\n";

    std::cout
        << "DAY 17: PERSISTENT SEARCH "
        << (passed
                ? "PASS"
                : "FAIL")
        << '\n';

    std::cout
        << "========================================\n";

    bufferPool.FlushAllPages();

    return passed ? 0 : 1;
}