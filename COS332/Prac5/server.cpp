#include <iostream>
#include <ldap.h>
#include <string.h>

void search_friend(LDAP* ldap, const std::string& name) {
    const char* base_dn = "ou=Friends,dc=example,dc=com";
    std::string filter = "(cn=" + name + ")";
    const char* attrs[] = {"telephoneNumber", NULL};

    LDAPMessage* res;
    int rc = ldap_search_ext_s(ldap, base_dn, LDAP_SCOPE_SUBTREE, filter.c_str(), 
                             const_cast<char**>(attrs), 0, NULL, NULL, NULL, 0, &res);
    if (rc != LDAP_SUCCESS) {
        std::cerr << "ldap_search_ext_s failed: " << ldap_err2string(rc) << std::endl;
        return;
    }

    int num_entries = ldap_count_entries(ldap, res);
    std::cout << "Found " << num_entries << " entries" << std::endl;

    for (LDAPMessage* entry = ldap_first_entry(ldap, res); 
         entry != NULL; 
         entry = ldap_next_entry(ldap, entry)) {
        
        char* dn = ldap_get_dn(ldap, entry);
        std::cout << "DN: " << dn << std::endl;
        ldap_memfree(dn);

        BerElement* ber;
        for (char* attr = ldap_first_attribute(ldap, entry, &ber); 
             attr != NULL; 
             attr = ldap_next_attribute(ldap, entry, ber)) {
            
            berval** vals = ldap_get_values_len(ldap, entry, attr);
            if (vals != NULL) {
                for (int i = 0; vals[i] != NULL; i++) {
                    std::cout << attr << ": " << vals[i]->bv_val << std::endl;
                }
                ldap_value_free_len(vals);
            }
            ldap_memfree(attr);
        }
        if (ber != NULL) ber_free(ber, 0);
    }
    ldap_msgfree(res);
}

void add_friend(LDAP* ldap, const std::string& name, const std::string& telephone) {
  std::string dn = "cn=" + name + ",ou=Friends,dc=example,dc=com";
  
  LDAPMod* mods[5];
  LDAPMod mod_cn, mod_sn, mod_telephone, mod_objectclass;

  char* cn_val[] = { const_cast<char*>(name.c_str()), NULL };
  mod_cn.mod_op = LDAP_MOD_ADD;
  mod_cn.mod_type = const_cast<char*>("cn");
  mod_cn.mod_values = cn_val;

  char* sn_val[] = { const_cast<char*>(name.c_str()), NULL };
  mod_sn.mod_op = LDAP_MOD_ADD;
  mod_sn.mod_type = const_cast<char*>("sn");
  mod_sn.mod_values = sn_val;

  char* tel_val[] = { const_cast<char*>(telephone.c_str()), NULL };
  mod_telephone.mod_op = LDAP_MOD_ADD;
  mod_telephone.mod_type = const_cast<char*>("telephoneNumber");
  mod_telephone.mod_values = tel_val;

  char* oc_val[] = { const_cast<char*>("inetOrgPerson"), NULL };
  mod_objectclass.mod_op = LDAP_MOD_ADD;
  mod_objectclass.mod_type = const_cast<char*>("objectClass");
  mod_objectclass.mod_values = oc_val;

  mods[0] = &mod_cn;
  mods[1] = &mod_sn;
  mods[2] = &mod_telephone;
  mods[3] = &mod_objectclass;
  mods[4] = NULL;

  int rc = ldap_add_ext_s(ldap, dn.c_str(), mods, NULL, NULL);
  if (rc != LDAP_SUCCESS) {
      std::cerr << "Error adding entry: " << ldap_err2string(rc) << std::endl;
  } else {
      std::cout << "Added: " << dn << std::endl;
  }
}

void prompt_and_add_friend(LDAP* ldap) {
  std::string name, telephone;
  std::cout << "\nEnter new friend's name: ";
  std::getline(std::cin, name);
  std::cout << "Enter telephone number: ";
  std::getline(std::cin, telephone);
  add_friend(ldap, name, telephone);
}

int main() {
    const char* ldap_uri = std::getenv("LDAP_URI");
    if (!ldap_uri) {
        ldap_uri = "ldap://127.0.0.1";
    }
    const int ldap_version = LDAP_VERSION3;
    const char* bind_dn = "cn=admin,dc=example,dc=com";
    const char* bind_pw = "3918";

    LDAP* ldap;
    int rc = ldap_initialize(&ldap, ldap_uri);
    if (rc != LDAP_SUCCESS) {
        std::cerr << "ldap_initialize failed: " << ldap_err2string(rc) << std::endl;
        return 1;
    }

    rc = ldap_set_option(ldap, LDAP_OPT_PROTOCOL_VERSION, &ldap_version);
    if (rc != LDAP_SUCCESS) {
        std::cerr << "ldap_set_option failed: " << ldap_err2string(rc) << std::endl;
        ldap_unbind_ext(ldap, NULL, NULL);
        return 1;
    }

    struct berval cred;
    cred.bv_val = const_cast<char*>(bind_pw);
    cred.bv_len = strlen(bind_pw);
    rc = ldap_sasl_bind_s(ldap, bind_dn, LDAP_SASL_SIMPLE, &cred, NULL, NULL, NULL);
    if (rc != LDAP_SUCCESS) {
        std::cerr << "ldap_simple_bind_s failed: " << ldap_err2string(rc) << std::endl;
        ldap_unbind_ext(ldap, NULL, NULL);
        return 1;
    }

    while (true) {
        std::cout << "\n1. Search for a friend\n2. Add a new friend\n3. Exit\nChoose: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: {
                std::string name;
                std::cout << "Enter name to search: ";
                std::getline(std::cin, name);
                search_friend(ldap, name);
                break;
            }
            case 2:
                prompt_and_add_friend(ldap);
                break;
            case 3:
                ldap_unbind_ext(ldap, NULL, NULL);
                return 0;
            default:
                std::cout << "Invalid choice.\n";
        }
    }
}