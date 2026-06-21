#include <iostream>  // Thay <stdio.h> bằng <iostream> cho C++
#include <algorithm> // Giữ lại để dùng max/min
#include <string>    // Thêm để dùng string cho mảng action

#define dungtichX 9
#define dungtichY 4
#define empty 0
#define muctieu 6
#define Maxlength 100

using namespace std; // Thêm để bỏ tiền tố std::

// Chuyển mảng action sang kiểu string
const string action[] = {"Trang thai 1", "do day binh X", "do day binh Y", 
                         "do het binh X", "do het binh Y", "do tu X qua Y", 
                         "do tu Y qua X"};

// Định nghĩa struct TrangThai
struct TrangThai {
    int x;
    int y;
};

// Trạng thái ban đầu
void makeNullState(TrangThai& state) { // Thay con trỏ * bằng tham chiếu &
    state.x = 0;
    state.y = 0;
}

// In trạng thái
void print_State(const TrangThai& state) { // Thay printf bằng cout
    cout << "\n X: " << state.x << " && Y: " << state.y;
}

// Kiểm tra trạng thái mục tiêu
bool goal_check(const TrangThai& state) { // Thay int bằng bool
    return (state.x == muctieu || state.y == muctieu);
}

// Đổ đầy X
bool fullX(const TrangThai& trangthai_hientai, TrangThai& ketqua) {
    if (trangthai_hientai.x < dungtichX) {
        ketqua.x = dungtichX;
        ketqua.y = trangthai_hientai.y;
        return true; // Thành công
    }
    return false; // Không thực hiện được
}

// Đổ đầy Y
bool fullY(const TrangThai& trangthai_hientai, TrangThai& ketqua) {
    if (trangthai_hientai.y < dungtichY) {
        ketqua.y = dungtichY;
        ketqua.x = trangthai_hientai.x;
        return true; // Thành công
    }
    return false; // Không thực hiện được
}

// Đổ hết X
bool emptyX(const TrangThai& trangthai_hientai, TrangThai& ketqua) {
    if (trangthai_hientai.x > 0) {
        ketqua.x = empty;
        ketqua.y = trangthai_hientai.y;
        return true; // Thành công
    }
    return false; // Không thực hiện được
}

// Đổ hết Y
bool emptyY(const TrangThai& trangthai_hientai, TrangThai& ketqua) {
    if (trangthai_hientai.y > 0) {
        ketqua.y = empty;
        ketqua.x = trangthai_hientai.x;
        return true; // Thành công
    }
    return false; // Không thực hiện được
}

// Đổ từ X qua Y
bool pourXY(const TrangThai& trangthai_hientai, TrangThai& ketqua) {
    if (trangthai_hientai.x > 0 && trangthai_hientai.y < dungtichY) {
        ketqua.x = max(trangthai_hientai.x - (dungtichY - trangthai_hientai.y), empty);
        ketqua.y = min(trangthai_hientai.x + trangthai_hientai.y, dungtichY);
        return true; // Thành công
    }
    return false; // Không thực hiện được
}

// Đổ từ Y qua X
bool pourYX(const TrangThai& trangthai_hientai, TrangThai& ketqua) {
    if (trangthai_hientai.y > 0 && trangthai_hientai.x < dungtichX) {
        ketqua.y = max(trangthai_hientai.y - (dungtichX - trangthai_hientai.x), empty);
        ketqua.x = min(trangthai_hientai.y + trangthai_hientai.x, dungtichX);
        return true; // Thành công
    }
    return false; // Không thực hiện được
}

// Gọi các phép toán
bool call_operator(const TrangThai& trangthai_hientai, TrangThai& ketqua, int luachon) {
    switch (luachon) {
        case 1: return fullX(trangthai_hientai, ketqua);
        case 2: return fullY(trangthai_hientai, ketqua);
        case 3: return emptyX(trangthai_hientai, ketqua);
        case 4: return emptyY(trangthai_hientai, ketqua);
        case 5: return pourXY(trangthai_hientai, ketqua);
        case 6: return pourYX(trangthai_hientai, ketqua);
        default:
            cout << "Loi!!!" << endl; // Thay printf bằng cout
            return false;
    }
}

// Hàm chính
int main() {
    TrangThai trangthai_hientai = {5, 4}, ketqua;
    cout << "Trang thai ban dau:"; // Thay printf bằng cout
    print_State(trangthai_hientai);

    for (int opt = 1; opt <= 6; opt++) {
        bool thuchien = call_operator(trangthai_hientai, ketqua, opt); // Thay int bằng bool
        if (thuchien) {
            cout << "\n Hanh dong " << action[opt] << " thanh cong";
            print_State(ketqua);
        } else {
            cout << "\n Hanh dong " << action[opt] << " khong thanh cong";
        }
    }
    cout << endl; // Thêm xuống dòng cuối cho đẹp
    return 0;
}