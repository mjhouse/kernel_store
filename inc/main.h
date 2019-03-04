
#define NULL 0;

struct hash_entry {
    hash_entry* right;
    hash_entry* left;

    char* key;
    char* val;
}

static hash_entry* root = 0;

hash_entry* find(char* k) {
    
    hash_entry* cur = root;
    
    while(cur != NULL) {
        if(strcmp(k,cur->key) < 0){
            cur = cur->left;
        }
        else if(strcmp(k,cur->key) > 0){
            cur = cur->right;
        }
        else {
            cur = NULL;
            break;
        }
    }

    return cur;
}

bool add(hash_entry* entry){

    if(entry == NULL)
        return false;

    hash_entry* cur = root;

    while(cur != NULL) {
        if(strcmp(k,cur->key) < 0){
            cur = cur->left;
        }
        else if(strcmp(k,cur->key) > 0){
            cur = cur->right;
        }
        else {
            break;
        }
    }

}

void remove(char* key){

}