#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR_SCREEN() system("cls")
    #define WAIT() Sleep(2000)
#else
    #include <unistd.h>
    #define CLEAR_SCREEN() system("clear")
    #define WAIT() sleep(2)
#endif

typedef struct {
    char username[50];
    char password[50];
} User;

typedef struct {
    char itemName[50];
    float price;
    int quantity;
} MenuItem;

typedef struct {
    char username[50];
    char itemName[50];
    int quantity;
    float totalPrice;
} Order;

typedef struct {
    char code[20];
    float discountPercentage;
} Coupon;

void registerUserOrAdmin(int isAdmin);
int login(int *isAdmin);
void userModule(char *username);
void adminModule();
void viewMenu();
void createMenu();
void placeOrder(char *username);
void viewOrderHistory();
void checkTotalSales();
void createCoupon();
float applyCoupon(char *couponCode);

int main() {
    int choice;
    int isAdmin;

    while (1) {
        CLEAR_SCREEN();
        printf("\nWelcome to FOODBANK\n");
        printf("1. Register as User\n");
        printf("2. Register as Admin\n");
        printf("3. Login\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                registerUserOrAdmin(0);
                break;
            case 2:
                registerUserOrAdmin(1);
                break;
            case 3:
                if (login(&isAdmin)) {
                    if (isAdmin) {
                        adminModule();
                    } else {
                        char username[50];
                        printf("Enter your username to continue: ");
                        scanf("%s", username);
                        getchar();
                        userModule(username);
                    }
                }
                break;
            case 4:
                printf("Exiting the program.\n");
                WAIT();
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
                WAIT();
        }
    }

    return 0;
}

void registerUserOrAdmin(int isAdmin) {
    CLEAR_SCREEN();
    FILE *file;
    char filename[20];
    strcpy(filename, isAdmin ? "admins.txt" : "users.txt");

    file = fopen(filename, "a+");
    if (file == NULL) {
        printf("Error: Could not open %s.\n", filename);
        WAIT();
        return;
    }

    User user;
    printf("Enter username: ");
    scanf("%s", user.username);
    printf("Enter password: ");
    scanf("%s", user.password);
    getchar();

    char fileUsername[50], filePassword[50];
    while (fscanf(file, "%s %s", fileUsername, filePassword) != EOF) {
        if (strcmp(fileUsername, user.username) == 0) {
            printf("Error: Username already exists. Please choose a different username.\n");
            WAIT();
            fclose(file);
            return;
        }
    }

    fseek(file, 0, SEEK_END);
    fprintf(file, "%s %s\n", user.username, user.password);
    printf("%s registered successfully!\n", isAdmin ? "Admin" : "User");
    WAIT();
    fclose(file);
}

int login(int *isAdmin) {
    CLEAR_SCREEN();
    FILE *file;
    char username[50], password[50];
    char fileUsername[50], filePassword[50];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);
    getchar();

    file = fopen("users.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%s %s", fileUsername, filePassword) != EOF) {
            if (strcmp(username, fileUsername) == 0 && strcmp(password, filePassword) == 0) {
                *isAdmin = 0;
                fclose(file);
                printf("Login successful! Welcome, User.\n");
                WAIT();
                return 1;
            }
        }
        fclose(file);
    }

    file = fopen("admins.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%s %s", fileUsername, filePassword) != EOF) {
            if (strcmp(username, fileUsername) == 0 && strcmp(password, filePassword) == 0) {
                *isAdmin = 1;
                fclose(file);
                printf("Login successful! Welcome, Admin.\n");
                WAIT();
                return 1;
            }
        }
        fclose(file);
    }

    printf("Invalid username or password.\n");
    WAIT();
    return 0;
}

void userModule(char *username) {
    int choice;
    do {
        CLEAR_SCREEN();
        printf("\nUser Module:\n");
        printf("1. View Menu\n");
        printf("2. Place Order\n");
        printf("3. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                viewMenu();
                break;
            case 2:
                placeOrder(username);
                break;
            case 3:
                printf("Logging out...\n");
                WAIT();
                return;
            default:
                printf("Invalid choice. Try again.\n");
                WAIT();
        }
    } while (1);
}

void adminModule() {
    int choice;
    do {
        CLEAR_SCREEN();
        printf("\nAdmin Module:\n");
        printf("1. Create Menu\n");
        printf("2. View Order History\n");
        printf("3. Check Total Sales\n");
        printf("4. Create Coupon Code\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                createMenu();
                break;
            case 2:
                viewOrderHistory();
                break;
            case 3:
                checkTotalSales();
                break;
            case 4:
                createCoupon();
                break;
            case 5:
                printf("Logging out...\n");
                WAIT();
                return;
            default:
                printf("Invalid choice. Try again.\n");
                WAIT();
        }
    } while (1);
}

void viewMenu() {
    FILE *file = fopen("menu.txt", "r+");
    if (file == NULL) {
        printf("Menu is empty. Please ask the admin to create a menu.\n");
        WAIT();
        return;
    }

    MenuItem item;
    CLEAR_SCREEN();
    printf("=======================================\n");
    printf("                MENU                   \n");
    printf("=======================================\n");
    while (fscanf(file, "%s %f %d", item.itemName, &item.price, &item.quantity) != EOF) {
        printf("Item: %-20s | Price: $%-6.2f | Available Quantity: %-3d\n", item.itemName, item.price, item.quantity);
    }
    printf("=======================================\n");

    fclose(file);
    printf("Press Enter to return...");
    getchar(); getchar();
}

void createMenu() {
    CLEAR_SCREEN();
    FILE *file = fopen("menu.txt", "a+");
    if (file == NULL) {
        printf("Error opening file.\n");
        WAIT();
        return;
    }

    MenuItem item;
    printf("Enter item name: ");
    scanf("%s", item.itemName);
    printf("Enter price: ");
    scanf("%f", &item.price);
    printf("Enter available quantity: ");
    scanf("%d", &item.quantity);

    fprintf(file, "%s %.2f %d\n", item.itemName, item.price, item.quantity);
    fclose(file);
    printf("Menu item added successfully.\n");
    WAIT();
}

void placeOrder(char *username) {
    FILE *menuFile = fopen("menu.txt", "r+");
    if (menuFile == NULL) {
        printf("Menu is empty. Please contact the admin.\n");
        WAIT();
        return;
    }

    char itemName[50];
    int quantity;
    float totalPrice = 0.0;
    int orderPlaced = 0;
    char couponCode[20];
    float discount = 0.0;

    printf("Enter the item name: ");
    scanf("%s", itemName);
    printf("Enter quantity: ");
    scanf("%d", &quantity);

    MenuItem item;
    int found = 0;
    long pos;
    while (!found && (pos = ftell(menuFile), fscanf(menuFile, "%s %f %d", item.itemName, &item.price, &item.quantity) != EOF)) {
        if (strcmp(item.itemName, itemName) == 0) {
            if (item.quantity >= quantity) {
                totalPrice = item.price * quantity;
                item.quantity -= quantity;
                fseek(menuFile, pos, SEEK_SET);
                fprintf(menuFile, "%s %.2f %d\n", item.itemName, item.price, item.quantity);
                found = 1;
                orderPlaced = 1;
            } else {
                printf("Error: Not enough quantity available.\n");
                WAIT();
                fclose(menuFile);
                return;
            }
        }
    }

    if (!orderPlaced) {
        printf("Error: Item not found.\n");
        WAIT();
        fclose(menuFile);
        return;
    }

    fclose(menuFile);

    printf("Enter coupon code (or press Enter to skip): ");
    getchar();
    fgets(couponCode, 20, stdin);
    couponCode[strcspn(couponCode, "\n")] = 0;

    if (strlen(couponCode) > 0) {
        discount = applyCoupon(couponCode);
        totalPrice -= totalPrice * (discount / 100);
    }

    FILE *orderFile = fopen("orders.txt", "a+");
    if (orderFile == NULL) {
        printf("Error saving order.\n");
        WAIT();
        return;
    }

    fprintf(orderFile, "%s %s %d %.2f\n", username, itemName, quantity, totalPrice);
    fclose(orderFile);

    // Displaying the bill
    CLEAR_SCREEN();
    printf("=======================================\n");
    printf("              BILL                     \n");
    printf("=======================================\n");
    printf("User: %-20s\n", username);
    printf("Item: %-20s\n", itemName);
    printf("Quantity: %-3d\n", quantity);
    printf("Subtotal: $%.2f\n", item.price * quantity);
    if (discount > 0) {
        printf("Discount Applied: %.2f%%\n", discount);
        printf("Discount Amount: $%.2f\n", item.price * quantity * (discount / 100));
    }
    printf("Total Price: $%.2f\n", totalPrice);
    printf("=======================================\n");

    // Wait for user to press Enter before returning to the menu
    printf("Press Enter to return to the menu...");
    getchar();
    getchar();
}

void viewOrderHistory() {
    FILE *file = fopen("orders.txt", "r");
    if (file == NULL) {
        printf("No orders have been placed yet.\n");
        WAIT();
        return;
    }

    Order order;
    CLEAR_SCREEN();
    printf("=======================================\n");
    printf("          ORDER HISTORY                \n");
    printf("=======================================\n");
    while (fscanf(file, "%s %s %d %f", order.username, order.itemName, &order.quantity, &order.totalPrice) != EOF) {
        printf("User: %-20s | Item: %-15s | Quantity: %-3d | Total Price: $%-6.2f\n",
               order.username, order.itemName, order.quantity, order.totalPrice);
    }
    printf("=======================================\n");

    fclose(file);
    printf("Press Enter to return...");
    getchar();
    getchar();
}

void checkTotalSales() {
    FILE *file = fopen("orders.txt", "r");
    if (file == NULL) {
        printf("No orders have been placed yet.\n");
        WAIT();
        return;
    }

    Order order;
    float totalSales = 0.0;
    while (fscanf(file, "%s %s %d %f", order.username, order.itemName, &order.quantity, &order.totalPrice) != EOF) {
        totalSales += order.totalPrice;
    }

    fclose(file);
    printf("Total Sales: $%.2f\n", totalSales);
    printf("Press Enter to return...");
    getchar();
    getchar();
}

void createCoupon() {
    CLEAR_SCREEN();
    FILE *file = fopen("coupons.txt", "a+");
    if (file == NULL) {
        printf("Error opening file.\n");
        WAIT();
        return;
    }

    Coupon coupon;
    printf("Enter coupon code: ");
    scanf("%s", coupon.code);
    printf("Enter discount percentage: ");
    scanf("%f", &coupon.discountPercentage);

    fprintf(file, "%s %.2f\n", coupon.code, coupon.discountPercentage);
    fclose(file);
    printf("Coupon created successfully.\n");
    WAIT();
}

float applyCoupon(char *couponCode) {
    FILE *file = fopen("coupons.txt", "r");
    if (file == NULL) {
        printf("No coupons available.\n");
        return 0.0;
    }

    Coupon coupon;
    while (fscanf(file, "%s %f", coupon.code, &coupon.discountPercentage) != EOF) {
        if (strcmp(coupon.code, couponCode) == 0) {
            fclose(file);
            printf("Coupon applied! Discount: %.2f%%\n", coupon.discountPercentage);
            WAIT();
            return coupon.discountPercentage;
        }
    }

    fclose(file);
    printf("Invalid coupon code.\n");
    WAIT();
    return 0.0;
}
