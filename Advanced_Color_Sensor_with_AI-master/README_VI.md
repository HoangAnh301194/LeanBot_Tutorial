# Hướng dẫn huấn luyện lại mô hình nhận diện màu cho Leanbot (Tiếng Việt)

> [English documentation / Tài liệu tiếng Anh](README.md)  
> Hướng dẫn này sử dụng mã nguồn có sẵn trong thư mục **Advanced_Color_Sensor_with_AI-master**, tập trung vào cảm biến **APDS9960**, thu thập dữ liệu RGB, huấn luyện **Decision Tree** bằng Python và triển khai mô hình lên Leanbot chạy Arduino.

## Mục lục

1. [Tổng quan và chuẩn bị](#1-tổng-quan-và-chuẩn-bị)
2. [Thu thập dữ liệu màu trên Leanbot](#2-thu-thập-dữ-liệu-màu-trên-leanbot)
3. [Chuẩn bị tập dữ liệu JSON](#3-chuẩn-bị-tập-dữ-liệu-json)
4. [Huấn luyện và đánh giá mô hình Decision Tree](#4-huấn-luyện-và-đánh-giá-mô-hình-decision-tree)
5. [Đưa mô hình xuống Leanbot](#5-đưa-mô-hình-xuống-leanbot)
6. [Tích hợp vào bài thi IoT hai Leanbot](#6-tích-hợp-vào-bài-thi-iot-hai-leanbot)
7. [Kiểm tra thực tế và xử lý lỗi](#7-kiểm-tra-thực-tế-và-xử-lý-lỗi)
8. [Checklist thực hành](#8-checklist-thực-hành)

---

## 1. Tổng quan và chuẩn bị

### 1.1. “Train lại cảm biến” là gì?

Cảm biến APDS9960 **không tự được huấn luyện**. Cảm biến đo phản hồi ánh sáng từ vật thể; chương trình dùng các giá trị đo để **huấn luyện lại bộ phân loại màu** cho điều kiện thực tế (khối màu, khoảng cách, góc đặt vật, ánh sáng).

Luồng xử lý:

```text
Khối màu thực tế
    ↓
Cảm biến APDS9960 + LED RGB trên Leanbot
    ↓
Ba đặc trưng: objRed, objGreen, objBlue
    ↓
Dữ liệu có nhãn: RED / GREEN / BLUE / ...
    ↓
Python (scikit-learn) → Decision Tree
    ↓
Xuất các luật if/else tương đương sang Arduino C++
    ↓
Leanbot nhận diện màu tại thời điểm chạy
```

Trong mã đo, Leanbot lần lượt bật LED đỏ, xanh lá, xanh dương rồi tắt LED để đo ánh sáng nền. Các đặc trưng được tính:

```cpp
ObjRed   = rRed   - rr;
ObjGreen = gGreen - gg;
ObjBlue  = bBlue  - bb;
```

Đây là **giá trị phản hồi đã trừ nền**, không phải mã RGB 8-bit chuẩn (0–255) của ảnh.

### 1.2. Các tệp quan trọng

| Tệp | Vai trò |
|---|---|
| [Code/samplingColor.ino](Code/samplingColor.ino) | Nạp lên Leanbot để lấy mẫu màu và in dữ liệu qua Serial |
| [Data/](Data/) | Các tệp JSON mẫu đã thu thập |
| [Code/dtree.py](Code/dtree.py) | Đọc JSON, chia dữ liệu, train Decision Tree, in các luật và đánh giá |
| [Code/template.ino](Code/template.ino) | Chương trình mẫu để thay hàm phân loại màu |
| [Code/classifiyColor.ino](Code/classifiyColor.ino) | Ví dụ đã chuyển Decision Tree thành hàm C++ (tên tệp trong repo được viết là `classifiyColor.ino`) |
| [README.md](README.md) | Hướng dẫn tiếng Anh gốc |

Cần có: Leanbot với APDS9960, cáp USB, Arduino IDE và thư viện Leanbot/APDS9960 đang dùng được trên máy; máy tính có Python, `pandas`, `numpy`, `matplotlib` và `scikit-learn`.

Cài thư viện Python nếu thiếu:

```bash
python -m pip install pandas numpy matplotlib scikit-learn
```

> **Lưu ý:** Ví dụ gốc huấn luyện **5 màu**: RED, YELLOW, GREEN, BLUE, PURPLE. Đối với bài IoT gồm `iot1.ino` và `iot2.ino`, có thể chỉ thu thập và huấn luyện **3 màu** RED, GREEN, BLUE cho phù hợp nhiệm vụ.

---

## 2. Thu thập dữ liệu màu trên Leanbot

### 2.1. Nạp chương trình lấy mẫu

1. Mở [samplingColor.ino](Code/samplingColor.ino) trong Arduino IDE.
2. Chọn đúng board và cổng COM/USB của Leanbot.
3. Nạp chương trình vào **Leanbot #1** (robot có nhiệm vụ nhận diện màu).
4. Mở **Serial Monitor** để quan sát dữ liệu đo.
5. Đặt khối màu trong vùng đo phía trước cảm biến, giữa hai càng gắp; tham khảo cách bố trí trong tài liệu gốc (khoảng cách thường trong khoảng 5 cm, sau đó có thể thử đến khoảng 5–8 cm nếu thực tế yêu cầu).

![Bố trí mẫu màu trước Leanbot](Docs/da.jpg)

### 2.2. Cấu hình số lượng màu

Trong `samplingColor.ino`, mã gốc có:

```cpp
#define objColorNum 5
const char* objColor[objColorNum] = {
  "RED", "YELLOW", "GREEN", "BLUE", "PURPLE"
};
const uint32_t objColorRGB[objColorNum] = {
  0xFF0000, 0xFFFF00, 0x00FF00, 0x0000FF, 0xFF00FF
};
int objColorIndex = 0;
```

Nếu chỉ train 3 màu cho bài IoT, thay **đồng bộ** tên màu và màu LED:

```cpp
#define objColorNum 3
const char* objColor[objColorNum] = {
  "RED", "GREEN", "BLUE"
};
const uint32_t objColorRGB[objColorNum] = {
  0xFF0000, 0x00FF00, 0x0000FF
};
int objColorIndex = 0;
```

Các tham số lấy mẫu gốc:

```cpp
const int POSITION_PER_COLOR = 20;
const int MEASURE_PER_POSITION = 1;
```

Tức là một lượt tự động lấy **20 mẫu/màu**. Với 5 màu được 100 mẫu; với 3 màu được 60 mẫu. Có thể tăng số vị trí/mẫu khi cần nhưng nên thay đổi vị trí và điều kiện thực sự, không chỉ đo lặp một khối đứng yên.

### 2.3. Ý nghĩa chính xác của các nút

Theo **mã `samplingColor.ino`** đang có trong repository:

| Nút | Tác dụng |
|---|---|
| **TB2A** | Lấy mẫu tại vị trí vật đang đặt; in một hoặc nhiều bản ghi ra Serial |
| **TB1A** | Chuyển **sang màu tiếp theo** trong danh sách nhãn |
| **TB1B** | Chuyển **về màu trước đó** |
| **Tự động** | Sau `POSITION_PER_COLOR` vị trí, vòng lặp tự chuyển sang màu kế tiếp |

Đáng chú ý: `objColorIndex` ban đầu bằng 0 nhưng đầu `loop()` gọi `changeObjColor(+1)`, nên với **danh sách 5 màu gốc** chương trình **bắt đầu bằng YELLOW**, không phải RED. Nếu đổi thành danh sách 3 màu ở trên, lần đầu sẽ bắt đầu bằng **GREEN**. Hãy **xem nhãn/LED đang chọn** trước mỗi lượt đo; có thể nhấn TB1A hoặc TB1B để chuyển đến đúng màu.

### 2.4. Cách lấy dữ liệu thực tế

1. Xem màu đang được chọn; đặt **đúng khối màu tương ứng** trước cảm biến.
2. Nhấn **TB2A** để đo một mẫu.
3. Di chuyển khối một chút (tịnh tiến, nghiêng nhẹ, đổi mặt, thay khoảng cách) rồi đo tiếp.
4. Thu đủ số mẫu của màu đó; chương trình sẽ tự chuyển màu khi kết thúc đủ `POSITION_PER_COLOR` vị trí, hoặc có thể chuyển thủ công bằng TB1A/TB1B.
5. Lặp lại cho các màu còn lại.
6. Thu thêm các lượt trong điều kiện sáng khác nhau, ưu tiên **điều kiện ánh sáng và hình dạng khối giống sân thi thực tế**.

Ví dụ đầu ra Serial:

```json
{
  "objColor": "GREEN",
  "objRed": 6,
  "objGreen": 23,
  "objBlue": 5
}
```

Các trường có ý nghĩa:

- `objColor`: nhãn **đúng** do người thu thập lựa chọn, không phải dự đoán của mô hình.
- `objRed`, `objGreen`, `objBlue`: dữ liệu đo để mô hình học.

**Quan trọng:** Nếu chọn nhãn GREEN nhưng lại đặt khối BLUE vào cảm biến, mẫu bị **gán nhãn sai** và việc huấn luyện sẽ bị ảnh hưởng.

### 2.5. Mẹo thu thập dữ liệu tốt

- Đo nhiều mặt của cùng một khối; nếu có, đo thêm các khối khác cùng màu.
- Thay đổi có kiểm soát khoảng cách, hướng vật và vị trí lệch tâm.
- Thu ở nhiều mức chiếu sáng tương tự nơi sẽ chạy robot.
- Đảm bảo số mẫu tương đối cân bằng giữa các màu.
- Để một buổi/vòng thu dữ liệu riêng dùng **kiểm tra cuối cùng**, không đưa tất cả vào train.
- Nếu cần nhận biết “không có khối” hoặc “không rõ màu”, phải thiết kế và thu mẫu riêng hoặc thêm cơ chế từ chối dự đoán; Decision Tree 3/5 lớp không tự hiểu `Unknown`.

---

## 3. Chuẩn bị tập dữ liệu JSON

### 3.1. Lưu đầu ra Serial

`samplingColor.ino` in từng đối tượng JSON và thêm dấu phẩy sau đối tượng. Do đó **không thể lưu nguyên toàn bộ log Serial thành một tệp JSON hợp lệ**.

Thực hiện:

1. Sao chép các bản ghi đối tượng từ Serial Monitor (bỏ phần thông báo khởi động không phải JSON).
2. Ghép các đối tượng vào một mảng: thêm dấu `[` ở đầu và `]` ở cuối.
3. Xóa dấu phẩy **sau đối tượng cuối cùng**.
4. Không để dấu phẩy thừa sau trường `objBlue`.
5. Lưu dạng UTF-8 với phần mở rộng `.json`.

Ví dụ hợp lệ:

```json
[
  {"objColor":"RED",   "objRed":19, "objGreen":1,  "objBlue":2},
  {"objColor":"GREEN", "objRed":6,  "objGreen":23, "objBlue":5},
  {"objColor":"BLUE",  "objRed":2,  "objGreen":14, "objBlue":31}
]
```

Có thể kiểm tra cú pháp trước khi train:

```bash
python -m json.tool "ten_file.json"
```

### 3.2. Cách tổ chức dữ liệu

Các tệp JSON mẫu trong [Data/](Data/) của repository là dữ liệu **5 màu** thu trong phòng, mỗi tệp gồm 100 mẫu. Khi train cho **3 màu**, không nên trộn nhầm các tệp 5 màu cũ vào bộ dữ liệu mới.

Ví dụ tổ chức:

```text
Advanced_Color_Sensor_with_AI-master/
├── Code/
│   ├── samplingColor.ino
│   ├── dtree.py
│   └── ...
├── Data/
│   ├── Indoor with Normal Lighting 1.json   # dữ liệu mẫu 5 màu
│   └── TrainMoi/                             # chỉ chứa dữ liệu train mới
│       ├── lan_01.json
│       ├── lan_02.json
│       └── lan_03.json
├── README.md
└── README_VI.md
```

**Lưu ý về đường dẫn:** `dtree.py` sử dụng `glob.glob("*.json")`, nghĩa là nó đọc tất cả tệp `.json` trong **thư mục làm việc hiện tại**, không tự động tìm trong `Data/`. Vì vậy:

- Muốn chạy đúng **dữ liệu mẫu**: vào thư mục `Data/`, chạy `python ../Code/dtree.py`.
- Muốn train **dữ liệu mới**: vào `Data/TrainMoi/`, chạy `python ../../Code/dtree.py`.

---

## 4. Huấn luyện và đánh giá mô hình Decision Tree

### 4.1. Chạy mã Python có sẵn

Sau khi cài đủ thư viện và tạo dữ liệu JSON trong `Data/TrainMoi/`:

```bash
cd Advanced_Color_Sensor_with_AI-master/Data/TrainMoi
python ../../Code/dtree.py
```

Trong Windows có thể dùng `py` thay `python` nếu môi trường đã được cấu hình như vậy.

### 4.2. Cách chương trình train

Mã nguồn [dtree.py](Code/dtree.py) thực hiện các bước chính:

1. Đọc và gộp các tệp JSON trong thư mục hiện tại bằng `pandas`.
2. Loại các bản ghi có ít nhất một thành phần RGB âm.
3. Lấy ba đầu vào: `objRed`, `objGreen`, `objBlue`.
4. Lấy nhãn mục tiêu: `objColor`.
5. Chia **80% train / 20% test** với `stratify=y` để giữ tỷ lệ các màu.
6. Huấn luyện cây quyết định với `DecisionTreeClassifier(max_depth=5)`.
7. In luật phân nhánh bằng `export_text(...)`, dự đoán tập test, hiển thị ma trận nhầm lẫn và lưu hình cây `dtree.jpg`.

Đoạn cốt lõi:

```python
X_train = Train[["objRed", "objGreen", "objBlue"]]
y_train = Train["objColor"]
X_test = Test[["objRed", "objGreen", "objBlue"]]

dtree_model = DecisionTreeClassifier(max_depth=5)
dtree_model.fit(X_train.values, y_train)

predicted = dtree_model.predict(X_test.values)
```

**Giải thích tham số:** `max_depth=5` giới hạn độ sâu cây để tránh quá phức tạp. Đây là lựa chọn của tài liệu mẫu, **không phải giá trị tối ưu mặc định cho mọi bộ dữ liệu**.

### 4.3. Cách đọc kết quả

Ma trận nhầm lẫn cho biết màu thật bị dự đoán thành màu nào. Trong `dtree.py`, `normalize="true"` chuẩn hóa **theo từng hàng nhãn thật** (hàng = nhãn thật, cột = nhãn dự đoán).

Cần theo dõi:

- **Accuracy:** tỷ lệ tổng số mẫu được nhận diện đúng.
- **Recall từng màu:** trong các vật thực sự có màu đó, bao nhiêu mẫu được nhận diện chính xác.
- **Các cặp dễ nhầm:** ví dụ BLUE/PURPLE hoặc GREEN/YELLOW.
- **Hiệu quả trên lượt đo mới:** robot có nhận diện đúng những khối **chưa từng xuất hiện trong tập train** không?

> **Lưu ý đánh giá:** Code gốc chia ngẫu nhiên các mẫu theo tỷ lệ 80/20. Các mẫu đo lặp cùng một vật trong cùng phiên có thể rất giống nhau, khiến accuracy test cao hơn kết quả thực tế. Để đánh giá nghiêm túc hơn, hãy **giữ nguyên một hoặc nhiều phiên đo hoàn toàn độc lập** làm tập kiểm tra cuối cùng.

### 4.4. Xem luật học được

Sau khi train, terminal in cây dạng:

```text
|--- objBlue <= 8.50
|   |--- objGreen <= 1.50
|   |   |--- class: RED
...
```

Đây chỉ là ví dụ **cú pháp**. Các ngưỡng thực tế phải lấy từ **kết quả huấn luyện mới**, không sao chép ngưỡng của mô hình mẫu.

---

## 5. Đưa mô hình xuống Leanbot

Leanbot không cần chạy Python hay scikit-learn khi thi. Ta chuyển cây đã train thành các điều kiện `if/else` tương đương trong Arduino C++.

### 5.1. Chuyển các luật sang C++

Sử dụng kết quả `export_text()` của chính bộ dữ liệu mới để viết hàm:

```cpp
String classifyColor(int objRed, int objGreen, int objBlue) {
  // Thay phần này bằng toàn bộ luật từ cây mới.
  // Không giữ nguyên dòng mẫu khi đưa vào thi.
  return "UNKNOWN";
}
```

**Cách chuyển ngưỡng về số nguyên:**

- `objBlue <= 8.50` → `objBlue <= 8`.
- `objBlue > 8.50` → `objBlue >= 9`.
- Với ngưỡng nguyên, ví dụ `<= 5.00`, dùng `<= 5`; nhánh đối diện `> 5.00` tương đương `>= 6`.

Phải giữ nguyên cấu trúc nhánh và lớp dự đoán. Sau khi chuyển, nên kiểm thử cùng dữ liệu đầu vào trên cả Python và hàm C++ để phát hiện lỗi chép điều kiện.

Xem [classifiyColor.ino](Code/classifiyColor.ino) để tham khảo một cây **5 màu** đã chuyển; **không dùng nguyên cây cũ** nếu vừa train dữ liệu mới.

### 5.2. Test độc lập bằng chương trình mẫu

1. Mở [template.ino](Code/template.ino).
2. Thay **toàn bộ** hàm `classifyColor(...)` mặc định bằng hàm vừa chuyển.
3. Nạp lên Leanbot #1.
4. Đặt lần lượt từng khối màu trước cảm biến.
5. Xem các giá trị `ObjRed`, `ObjGreen`, `ObjBlue` và dòng `Detect Jenga Block:` trên Serial Monitor.
6. Đổi vị trí khối, kiểm tra lặp lại dưới ánh sáng sân thi.

Ví dụ đầu ra mong muốn:

```text
ObjRed   = 6
ObjGreen = 23
ObjBlue  = 5
Detect Jenga Block: GREEN
```

Không dùng `template.ino` nguyên bản để đánh giá, vì hàm mẫu trong tệp đó **luôn trả `"RED"`**.

---

## 6. Tích hợp vào bài thi IoT hai Leanbot

Trong [../Leanbot_IoT/iot1.ino](../Leanbot_IoT/iot1.ino), Leanbot #1 đang nhận diện theo luật thủ công:

```cpp
if (ObjRed > ObjGreen && ObjRed > ObjBlue && ObjRed > 3) {
  return "Red";
}
```

Nếu chuyển sang Decision Tree mới:

1. **Giữ nguyên** `Obj_readRGB()` và phần điều khiển Wi-Fi/IoT.
2. Chèn hàm `classifyColor(int, int, int)` với các điều kiện từ cây mới.
3. **Thay thế** phần cài đặt hiện tại của `Obj_detectColor()` bằng hàm bọc bên dưới (không tạo thêm hàm thứ hai cùng tên).
4. Giữ nguyên các lệnh `IoTColor1.write(detectedColor)`, `IoTColor2.write(...)` và `IoTColor3.write(...)` trong nhiệm vụ.

```cpp
String Obj_detectColor() {
  Obj_readRGB();
  String prediction = classifyColor(ObjRed, ObjGreen, ObjBlue);

  // iot2.ino kiểm tra chuỗi phân biệt chữ hoa/thường:
  if (prediction == "RED")   return "Red";
  if (prediction == "GREEN") return "Green";
  if (prediction == "BLUE")  return "Blue";
  return "Unknown";
}
```

**Rất quan trọng:** `iot2.ino` so sánh các chuỗi `"Red"`, `"Green"`, `"Blue"`, `"Unknown"` (chữ cái đầu viết hoa). Trong khi dữ liệu train và ví dụ Decision Tree dùng `"RED"`, `"GREEN"`, `"BLUE"` (toàn chữ hoa). Nếu ghi thẳng `"GREEN"` lên IoT mà không chuyển kiểu chữ, Leanbot #2 sẽ không xử lý đúng.

Ngoài ra, cây 3 màu **luôn chọn một trong ba màu** đối với đầu vào bất kỳ, trừ khi có cơ chế từ chối riêng. Vì thế, nhánh `Unknown` ở trên **chỉ xử lý chuỗi nhãn không hợp lệ**, không tự phát hiện vật lạ. Muốn phát hiện vật lạ cần bổ sung phép thử độ tin cậy, kiểm tra biên dữ liệu hoặc mô hình có lớp đối tượng khác.

**Khuyến nghị:** Chỉ thay thuật toán phân loại trên Leanbot #1 sau khi đã test độc lập. Không sửa đồng thời logic giao thông giữa hai robot khi đang đánh giá độ chính xác màu.

---

## 7. Kiểm tra thực tế và xử lý lỗi

| Hiện tượng | Nguyên nhân cần kiểm tra | Hướng xử lý |
|---|---|---|
| Dữ liệu RGB gần bằng 0 | Vật quá xa, lệch vị trí, ánh sáng yếu, cảm biến không đọc được | Đặt lại đúng vị trí, kiểm tra APDS9960 và LED |
| Màu BLUE hay bị nhầm PURPLE | Hai nhóm dữ liệu chồng lấn, khác khoảng cách hoặc ánh sáng | Thu thêm mẫu ở các tình huống nhầm lẫn |
| Kết quả đúng lúc test nhưng sai trên sân | Dữ liệu train chưa đại diện cho điều kiện thi | Thu thêm phiên đo ở sân thực tế, kiểm tra độc lập |
| Python báo lỗi JSON | Có dấu phẩy cuối hoặc dòng Serial không phải JSON | Kiểm tra bằng `python -m json.tool` |
| Python không tìm thấy dữ liệu | `glob("*.json")` đọc sai thư mục | Chạy từ thư mục chứa các tệp JSON |
| Python thiếu thư viện | Chưa cài môi trường phụ thuộc | Cài `pandas numpy matplotlib scikit-learn` |
| Robot #2 không phản ứng sau khi phân loại | Chuỗi `GREEN` khác `Green` | Chuẩn hóa chuỗi trả về IoT |
| Chờ cảm biến không kết thúc | `while (!APDS.colorAvailable())` không có timeout | Kiểm tra kết nối cảm biến; bổ sung timeout cho hệ thống vận hành chính |

### Lưu ý về chất lượng mô hình

Dữ liệu ví dụ trong repo có thể giúp học cách huấn luyện, nhưng **không chứng minh mô hình sẽ chính xác trên một Leanbot khác hoặc trên sân khác**. Điều kiện đo và vật thể phải được kiểm chứng lại.

Khi có thời gian, nên so sánh **luật thủ công hiện tại** với **Decision Tree huấn luyện mới** trên cùng một tập kiểm tra độc lập. Chỉ thay thuật toán chính nếu kết quả cải thiện rõ ràng.

---

## 8. Checklist thực hành

- [ ] Nạp `samplingColor.ino` lên Leanbot #1, mở Serial Monitor.
- [ ] Chọn đúng danh sách màu và xác nhận nhãn trước khi đo.
- [ ] Thu đủ mẫu mỗi màu, có thay đổi vị trí/góc/ánh sáng.
- [ ] Làm sạch log và lưu các tệp JSON hợp lệ.
- [ ] Đặt dữ liệu mới trong thư mục riêng, không trộn nhầm dữ liệu mẫu cũ.
- [ ] Chạy `dtree.py`, đọc ma trận nhầm lẫn và luật cây.
- [ ] Chuyển luật cây mới sang hàm `classifyColor(...)` trong C++.
- [ ] Test bằng `template.ino` trên Leanbot thực tế.
- [ ] Tích hợp vào `iot1.ino`, giữ đúng định dạng màu cho `iot2.ino`.
- [ ] Test lần cuối trên sân thật và lưu kết quả kiểm thử.

---

## Tham khảo

- [README gốc tiếng Anh](README.md).
- [samplingColor.ino](Code/samplingColor.ino), [dtree.py](Code/dtree.py), [template.ino](Code/template.ino), [classifiyColor.ino](Code/classifiyColor.ino).
- [Tài liệu DecisionTreeClassifier của scikit-learn](https://scikit-learn.org/stable/modules/generated/sklearn.tree.DecisionTreeClassifier.html).

> Tài liệu tiếng Việt này giải thích lại quy trình trong README tiếng Anh, đồng thời bổ sung các lưu ý tích hợp với bài thi IoT hai Leanbot. Các hướng dẫn bổ sung được nêu rõ dưới dạng khuyến nghị; không đồng nghĩa các tính năng đó đã có sẵn trong mã nguồn.
