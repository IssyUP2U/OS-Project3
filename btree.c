//ims190003
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "index.h"

#define T 10 //minimum degree

//initialize node
static void init_node(btree_node_t *n, uint64_t id, uint64_t parent)
{
    n->blockID = id;
    n->parentNode = parent;
    n->keyCount = 0;
    memset(n->keys, 0, sizeof(n->keys));
    memset(n->values, 0, sizeof(n->values));
    memset(n->children, 0, sizeof(n->children));
}

//search node values
static int search_node(FILE *fp, uint64_t blockID,
                       uint64_t key, uint64_t *value_out)
{
    if (blockID == 0)
        return 0;

    btree_node_t node;
    read_node(fp, blockID, &node);

    int i = 0;
    while (i < node.keyCount && key > node.keys[i])
        i++;

    if (i < node.keyCount && key == node.keys[i]) {
        *value_out = node.values[i];
        return 1;
    }

    return search_node(fp, node.children[i], key, value_out);
}

//search tree
int btree_search(const char *filename, uint64_t key)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) return 1;

    index_header_t hdr;
    read_header(fp, &hdr);

    uint64_t value;
    int found = search_node(fp, hdr.rootID, key, &value);

    fclose(fp);

    if (found) {
        printf("%lu %lu\n", key, value);
        return 0;
    }

    fprintf(stderr, "Key not found\n");
    return 1;
}

//split child node
static void split_child(FILE *fp, index_header_t *hdr,
                        btree_node_t *parent, int idx,
                        btree_node_t *child)
{
    btree_node_t new_node;
    init_node(&new_node, hdr->nextBlock++, parent->blockID);

    //move upper half to new node
    new_node.keyCount = T - 1;
    for (int j = 0; j < T - 1; j++) {
        new_node.keys[j] = child->keys[j + T];
        new_node.values[j] = child->values[j + T];
    }

    for (int j = 0; j < T; j++) {
        new_node.children[j] = child->children[j + T];
    }

    child->keyCount = T - 1;

    //shift parents children
    for (int j = parent->keyCount; j >= idx + 1; j--)
        parent->children[j + 1] = parent->children[j];

    parent->children[idx + 1] = new_node.blockID;

    //shift parent keys
    for (int j = parent->keyCount - 1; j >= idx; j--) {
        parent->keys[j + 1] = parent->keys[j];
        parent->values[j + 1] = parent->values[j];
    }

    parent->keys[idx] = child->keys[T - 1];
    parent->values[idx] = child->values[T - 1];
    parent->keyCount++;

    write_node(fp, child);
    write_node(fp, &new_node);
}

//insert when node not full
static void insert_nonfull(FILE *fp, index_header_t *hdr,
                           btree_node_t *node,
                           uint64_t key, uint64_t value)
{
    int i = node->keyCount - 1;

    if (node->children[0] == 0) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            node->values[i + 1] = node->values[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->values[i + 1] = value;
        node->keyCount++;
        write_node(fp, node);
    } else {
        while (i >= 0 && key < node->keys[i])
            i--;
        i++;

        btree_node_t child;
        read_node(fp, node->children[i], &child);

        if (child.keyCount == MAX_KEYS) {
            split_child(fp, hdr, node, i, &child);
            write_node(fp, node);

            if (key > node->keys[i])
                i++;
        }

        read_node(fp, node->children[i], &child);
        insert_nonfull(fp, hdr, &child, key, value);
    }
}

//insert
int btree_insert(const char *filename, uint64_t key, uint64_t value)
{
    FILE *fp = fopen(filename, "r+b");
    if (!fp) return 1;

    index_header_t hdr;
    read_header(fp, &hdr);

    if (hdr.rootID == 0) {
        btree_node_t root;
        init_node(&root, hdr.nextBlock++, 0);
        root.keys[0] = key;
        root.values[0] = value;
        root.keyCount = 1;
        hdr.rootID = root.blockID;

        write_node(fp, &root);
        write_header(fp, &hdr);
        fclose(fp);
        return 0;
    }

    btree_node_t root;
    read_node(fp, hdr.rootID, &root);

    if (root.keyCount == MAX_KEYS) {
        btree_node_t new_root;
        init_node(&new_root, hdr.nextBlock++, 0);
        new_root.children[0] = root.blockID;
        root.parentNode = new_root.blockID;

        split_child(fp, &hdr, &new_root, 0, &root);
        insert_nonfull(fp, &hdr, &new_root, key, value);

        hdr.rootID = new_root.blockID;
        write_node(fp, &new_root);
    } else {
        insert_nonfull(fp, &hdr, &root, key, value);
    }

    write_header(fp, &hdr);
    fclose(fp);
    return 0;
}

/* ------------------------------------------------------------
   TRAVERSAL (print/extract)
------------------------------------------------------------ */

static void traverse(FILE *fp, uint64_t blockID,
                     void (*visit)(uint64_t, uint64_t, void *),
                     void *ctx)
{
    if (blockID == 0)
        return;

    btree_node_t node;
    read_node(fp, blockID, &node);

    for (int i = 0; i < node.keyCount; i++) {
        traverse(fp, node.children[i], visit, ctx);
        visit(node.keys[i], node.values[i], ctx);
    }
    traverse(fp, node.children[node.keyCount], visit, ctx);
}

/* ------------------------------------------------------------
   PRINT
------------------------------------------------------------ */

static void print_pair(uint64_t k, uint64_t v, void *ctx)
{
    (void)ctx;
    printf("%lu %lu\n", k, v);
}

int btree_print(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) return 1;

    index_header_t hdr;
    read_header(fp, &hdr);
    traverse(fp, hdr.rootID, print_pair, NULL);

    fclose(fp);
    return 0;
}

/* ------------------------------------------------------------
   EXTRACT
------------------------------------------------------------ */

static void extract_pair(uint64_t k, uint64_t v, void *ctx)
{
    FILE *out = ctx;
    fprintf(out, "%lu,%lu\n", k, v);
}

int btree_extract(const char *filename, const char *outname)
{
    if (access(outname, F_OK) == 0)
        return 1;

    FILE *fp = fopen(filename, "rb");
    FILE *out = fopen(outname, "w");
    if (!fp || !out) return 1;

    index_header_t hdr;
    read_header(fp, &hdr);
    traverse(fp, hdr.rootID, extract_pair, out);

    fclose(fp);
    fclose(out);
    return 0;
}

/* ------------------------------------------------------------
   LOAD (CSV)
------------------------------------------------------------ */

int btree_load_csv(const char *filename, const char *csvfile)
{
    FILE *csv = fopen(csvfile, "r");
    if (!csv) return 1;

    char line[256];
    while (fgets(line, sizeof(line), csv)) {
        char *comma = strchr(line, ',');
        if (!comma) {
            fclose(csv);
            return 1;
        }
        *comma = '\0';
        uint64_t k = strtoull(line, NULL, 10);
        uint64_t v = strtoull(comma + 1, NULL, 10);
        btree_insert(filename, k, v);
    }

    fclose(csv);
    return 0;
}
