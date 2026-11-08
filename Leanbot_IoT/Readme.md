# Leanbot IoT – Giao tiếp và phối hợp hai robot

Tài liệu hướng dẫn ví dụ **AIROC26 Demo 01** sử dụng Leanbot và thư viện `LeanbotIoT.h` để hai robot trao đổi trạng thái qua một **IoT Asset** dùng chung. Các ví dụ được đối chiếu với [`iot1.ino`](./iot1.ino) và [`iot2.ino`](./iot2.ino).

> **Phân biệt:** `LbIoT`, `LbMotion`, `LbIRLine`, `LbGripper`… là các đối tượng/API từ thư viện. `Forward()`, `Obj_detectColor()`, `WaitIoTColor1()`… là **hàm do chương trình ví dụ tự định nghĩa**, không phải API có sẵn.

## 1. Mục tiêu và kiến trúc

- **Leanbot 1 (`iot1.ino`)** đi theo hành trình `CRL → G3 → E4 → H2 → CRL`, nhận diện màu vật tại ba vị trí, gửi `Color1`, `Color2`, `Color3` lên IoT và cập nhật trạng thái vùng xung đột qua `TrackingLb1`.
- **Leanbot 2 (`iot2.ino`)** đến vị trí chờ, đọc kết quả màu, lấy khối trung hòa ở `S1` khi gặp `Green` hoặc `S3` khi gặp `Blue`, sau đó giao đến các vị trí tương ứng; đọc `TrackingLb1` để chờ khi Leanbot 1 còn chiếm vùng xung đột.
- Hai robot dùng **cùng Asset ID** trên nền tảng [IoT Pythaverse](https://iot.pythaverse.space/). Dữ liệu trao đổi qua các *attribute* trên Asset, không truyền trực tiếp từ robot này sang robot kia.

```text
Leanbot 1 ── write(Color1/2/3, TrackingLb1) ──► IoT Asset
Leanbot 2 ◄──────── read(Color1/2/3, TrackingLb1) ── IoT Asset
```

## 2. Chuẩn bị và cấu hình

**Thư viện dùng trong ví dụ:**

```cpp
#include <Leanbot.h>
#include "LeanbotIoT.h"
// Riêng iot1.ino cần thêm:
#include <Arduino_APDS9960.h>
```

**Cấu hình Wi-Fi và Asset ID** trong **cả hai** file `.ino`:

```cpp
#define WIFI_SSID      F("YOUR_WIFI_SSID")
#define WIFI_PASSWORD  F("YOUR_WIFI_PASSWORD")
#define IOT_ASSET_ID   F("YOUR_ASSET_ID")

auto IoTColor1 = LbIoT.Asset.attribute(F("Color1"));
auto IoTColor2 = LbIoT.Asset.attribute(F("Color2"));
auto IoTColor3 = LbIoT.Asset.attribute(F("Color3"));
auto IoTTrackingLb1 = LbIoT.Asset.attribute(F("TrackingLb1"));

void setup() {
  Leanbot.begin();
  LbIRLine.doManualCalibration(TB1A);
  // Chỉ Leanbot 1 dùng cảm biến màu:
  // APDS.begin();
  LbIoT.Wifi.begin(WIFI_SSID, WIFI_PASSWORD);
  LbIoT.Asset.begin(IOT_ASSET_ID);
}
```

Các dòng trên minh họa phần khởi tạo; hãy giữ nguyên những thao tác `setup()` khác trong hai file nguồn. **Không đưa mật khẩu Wi-Fi thật lên GitHub**; dùng thông tin cấu hình phù hợp với môi trường triển khai.

## 3. API IoT – Input, Output và cách sử dụng

| API | Input | Output / tác dụng | Ví dụ |
|---|---|---|---|
| `LbIoT.Wifi.begin(ssid, password)` | SSID và mật khẩu Wi-Fi | Bắt đầu quá trình kết nối Wi-Fi | `LbIoT.Wifi.begin(WIFI_SSID, WIFI_PASSWORD);` |
| `LbIoT.Asset.begin(assetId)` | ID của Asset | Khởi tạo kênh làm việc với Asset | `LbIoT.Asset.begin(IOT_ASSET_ID);` |
| `LbIoT.Asset.attribute(name)` | Tên thuộc tính | Trả về đối tượng đại diện thuộc tính | `auto color = LbIoT.Asset.attribute(F("Color1"));` |
| `attribute.write(value)` | Giá trị gửi, ví dụ chuỗi | Cập nhật giá trị thuộc tính lên IoT | `IoTColor1.write("Green");` |
| `attribute.read()` | Không có | Đọc giá trị thuộc tính; trong ví dụ được gán vào `String` | `String color = IoTColor1.read();` |

**Quy ước thuộc tính trong chương trình:**

| Attribute | Giá trị trong ví dụ | Ý nghĩa |
|---|---|---|
| `Color1`, `Color2`, `Color3` | `waiting` | Chưa có kết quả nhận diện tại vị trí tương ứng |
| `Color1`, `Color2`, `Color3` | `Red`, `Green`, `Blue`, `Unknown` | Kết quả nhận diện màu (có phân biệt chữ hoa/chữ thường) |
| `TrackingLb1` | `ConflictZone1` | Leanbot 1 thông báo đang chiếm vùng xung đột số 1 |
| `TrackingLb1` | `NoConflict` | Leanbot 1 thông báo đã giải phóng vùng xung đột |

Tên `Color1`, `Color2`, `Color3`, `TrackingLb1` và các giá trị chuỗi nói trên là **quy ước ứng dụng**, không phải tên thuộc tính cố định của thư viện.

**Gửi dữ liệu – Leanbot 1:**

```cpp
String detectedColor = Obj_detectColor();
IoTColor1.write(detectedColor);
IoTTrackingLb1.write("ConflictZone1");
// Khi đã thực sự ra khỏi vùng xung đột:
IoTTrackingLb1.write("NoConflict");
```

**Nhận dữ liệu – Leanbot 2:**

```cpp
String color = IoTColor1.read();
if (color == "Green") {
  FetchGreen();
} else if (color == "Blue") {
  FetchBlue();
}

while (IoTTrackingLb1.read() == "ConflictZone1") {
  LbDelay(10);
}
// Chỉ tiếp tục sau khi điều kiện an toàn được xác nhận.
```

## 4. API Leanbot được gọi trực tiếp trong ví dụ

### 4.1. Chuyển động (`LbMotion`)

| API | Input | Output / tác dụng |
|---|---|---|
| `LbMotion.runLR(left, right)` | Giá trị điều khiển bánh trái và bánh phải | Ra lệnh vận tốc cho hai bánh |
| `LbMotion.waitDistanceMm(mm)` | Quãng đường, đơn vị mm | Chờ đến khi đạt quãng đường được yêu cầu |
| `LbMotion.waitRotationDeg(deg)` | Góc quay, đơn vị độ | Chờ đến khi đạt góc quay được yêu cầu |
| `LbMotion.stopAndWait()` | Không | Ra lệnh dừng và chờ robot dừng |
| `LbMotion.getDistanceMm()` | Không | Trả về quãng đường robot đo được (mm) |

```cpp
// Tiến 100 mm, rồi dừng
LbMotion.runLR(2000, 2000);
LbMotion.waitDistanceMm(100);
LbMotion.stopAndWait();

// Quay tại chỗ, rồi dừng
LbMotion.runLR(2000, -2000);
LbMotion.waitRotationDeg(90);
LbMotion.stopAndWait();
```

`2000` là giá trị điều khiển tốc độ dùng trong hai file ví dụ; **không tự quy đổi thành RPM/PWM** nếu chưa xác minh đơn vị từ thư viện Leanbot đang cài. Trong hàm `Rotate(speed, deg)` của ví dụ, dấu của `speed` quyết định chiều quay của hai bánh.

### 4.2. Dò line, cảm biến màu, LED, tay gắp và nhiệm vụ

| API | Input | Output / tác dụng |
|---|---|---|
| `Leanbot.begin()` | Không | Khởi tạo Leanbot |
| `LbIRLine.doManualCalibration(TB1A)` | Nút hiệu chuẩn | Thực hiện hiệu chuẩn dò line |
| `LbIRLine.setThreshold(t1,t2,t3,t4)` | Bốn ngưỡng | Thiết lập ngưỡng cảm biến (trong code đang để comment) |
| `LbIRLine.read()` | Không | Trả về mẫu bit của cảm biến dò line |
| `LbIRLine.displayOnRGB(color)` | Màu LED | Hiển thị trạng thái cảm biến |
| `APDS.begin()` | Không | Khởi tạo cảm biến màu APDS9960 (Leanbot 1) |
| `APDS.colorAvailable()` | Không | Trả về điều kiện có dữ liệu màu sẵn sàng |
| `APDS.readColor(r, g, b)` | Ba biến nhận dữ liệu | Đưa các thành phần RGB vào biến tham chiếu |
| `LbRGB.fillColor(color)` | Màu | Chọn màu LED RGB |
| `LbRGB.show()` | Không | Cập nhật trạng thái LED |
| `LbRGB.getBrightness()` | Không | Đọc độ sáng LED hiện tại |
| `LbRGB.setBrightness(value)` | Độ sáng | Cài đặt độ sáng LED |
| `LbGripper.open()` | Không | Mở tay gắp |
| `LbGripper.close(50)` | Tham số đóng tay gắp | Đóng tay gắp theo tham số |
| `LbMission.begin(TB1A + TB1B)` | Tổ hợp nút | Bắt đầu nhiệm vụ theo chương trình |
| `LbMission.end()` | Không | Kết thúc nhiệm vụ |
| `LbDelay(ms)` | Thời gian, đơn vị ms | Chờ theo thời gian yêu cầu |

## 5. Hàm tự định nghĩa trong `iot1.ino` / `iot2.ino`

Các hàm dưới đây **không phải API dựng sẵn**. Chúng được viết trên cơ sở các API ở mục 3 và 4.

| Hàm | Input | Output / tác dụng | File |
|---|---|---|---|
| `Forward(speed, distanceMm)` | Tốc độ, quãng đường mm | Tiến thẳng rồi dừng; trả về `void` | Cả hai |
| `Backward(speed, distanceMm)` | Tốc độ, quãng đường mm | Lùi rồi dừng; `void` | Cả hai |
| `Rotate(speed, deg)` | Giá trị tốc độ có dấu, góc độ | Quay tại chỗ rồi dừng; `void` | Cả hai |
| `runFollowLine()` | Không | Đọc bit cảm biến và điều chỉnh từng bánh; `void` | Cả hai |
| `followLineDistance(distanceMm)` | Quãng đường mm | Bám line theo quãng đường đặt trước; `void` | Cả hai |
| `followLineUntilJunction()` | Không | Bám line đến khi `LbIRLine.read() == 0b1111`; `void` | Cả hai |
| `followLineMultipleJunctions(junctionCount)` | Số giao lộ | Đi qua số giao lộ được đặt; `void` | Cả hai |
| `goToJunction()` | Không | Đến giao lộ, đi thêm 20 mm rồi dừng; `void` | Cả hai |
| `_coreRead(r,g,b,ledColor)` | Tham chiếu RGB, màu LED | Đọc RGB dưới ánh sáng LED tương ứng; `void` | Robot 1 |
| `Obj_readRGB()` | Không | Đo màu, trừ ánh sáng môi trường, cập nhật `ObjRed/ObjGreen/ObjBlue`; `void` | Robot 1 |
| `BasicObj_isRed/Green/Blue()` | Không | Trả về `boolean` xác định màu chủ đạo | Robot 1 |
| `Obj_detectColor()` | Không | Trả về `String`: `Red` / `Green` / `Blue` / `Unknown` | Robot 1 |
| `HandleColor(detectedColor)` | Chuỗi màu | Nếu màu đỏ, đẩy khối rồi lùi; `void` | Robot 1 |
| `WaitIoTColor1/2/3()` | Không | Chờ dữ liệu `Color1/2/3` khác `waiting`; `void` | Robot 2 |
| `HandleColor1/2/3()` | Không | Đọc IoT và thực hiện hành động theo màu; `void` | Robot 2 |
| `FetchGreen()` / `FetchBlue()` | Không | Thực hiện chuỗi di chuyển đến lấy khối tương ứng; `void` | Robot 2 |
| `DeliverNeutralizer()` | Không | Tiến, thả khối, lùi; `void` | Robot 2 |
| `WaitUntil_Leanbot1_outside_ConflictZone1()` | Không | Chờ cho đến khi `TrackingLb1` khác `ConflictZone1`; `void` | Robot 2 |

Các hàm định tuyến riêng như `FromCRLtoG3()`, `FromG3toE4()`, `WaitPointToG3()`, `G3ToWaitPoint()`, `WaitPointToE4()`… cũng là hàm ứng dụng; góc quay và quãng đường được hiệu chỉnh theo sa bàn của ví dụ.

## 6. Quy trình chạy thử

1. Bảo đảm cả hai Leanbot kết nối được Wi-Fi, cùng trỏ về **một IoT Asset** có đủ `Color1`, `Color2`, `Color3` và `TrackingLb1`.
2. Nạp `iot1.ino` cho Leanbot 1 và `iot2.ino` cho Leanbot 2; đặt hai robot đúng vị trí khởi đầu trên sa bàn, hiệu chuẩn cảm biến dò line theo chương trình.
3. Khởi động Leanbot 1: `setup()` đặt các giá trị `Color1/2/3 = waiting` và `TrackingLb1 = NoConflict`. Cần bảo đảm những lần ghi này đã được máy chủ nhận trước khi Robot 2 bắt đầu chờ.
4. Khởi động nhiệm vụ bằng tổ hợp `TB1A + TB1B` như code gốc. Leanbot 1 đến lần lượt G3, E4, H2, nhận diện màu rồi ghi thuộc tính tương ứng.
5. Leanbot 2 đọc kết quả từng vị trí: màu xanh lá lấy khối tại S1; màu xanh dương lấy tại S3; `Red` hoặc `Unknown` không lấy khối theo logic hiện tại. Trước một số chặng giao khối, robot kiểm tra `TrackingLb1`.
6. Quan sát các thuộc tính trên bảng quản trị IoT và Serial Monitor của Leanbot 2 (`HandleColor1/2/3()` có `Serial.println(detectedColor)`), kiểm tra robot chạy đúng theo trạng thái nhận được.

**Luồng dữ liệu minh họa:**

```text
Robot 1: Obj_detectColor() → IoTColor1.write("Green")
            ↓ (máy chủ lưu Color1)
Robot 2: IoTColor1.read() → "Green" → FetchGreen()
         → kiểm tra TrackingLb1 → đến G3 → thả khối
```

## 7. Giới hạn của chương trình mẫu và lưu ý an toàn

- Các vòng chờ `while (IoTColorX.read() == "waiting")` và `while (IoTTrackingLb1.read() == "ConflictZone1")` **không có timeout**. Nếu mạng mất hoặc dữ liệu không cập nhật, Robot 2 có thể chờ vô hạn. Nên bổ sung timeout, kiểm tra kết nối và trạng thái lỗi trước khi dùng thực tế.
- `WaitUntil_Leanbot1_outside_ConflictZone1()` chỉ kiểm tra **giá trị trạng thái từ IoT**, không phát hiện trực tiếp người/vật cản, không thay thế cơ chế chống va chạm và không bảo đảm dữ liệu luôn mới.
- Trong `iot1.ino`, `TrackingLb1` được ghi `"NoConflict"` trong `FromH2toCRL()` **trước** lệnh `Forward(speed, 230)`. Cần xác minh đúng hình học sa bàn để bảo đảm thời điểm công bố giải phóng vùng là an toàn.
- Phần nhận diện màu gốc chỉ xét `Red`, `Green`, `Blue` hoặc `Unknown`, dùng phép so sánh thành phần RGB chủ đạo; chưa phải bộ nhận diện đủ năm màu và có thể nhạy với điều kiện chiếu sáng.
- Các vòng chờ dữ liệu APDS và giao lộ trong mẫu chưa có xử lý lỗi/timeout. Khi thử nghiệm, nên có cơ chế dừng an toàn.
- Các hàm điều khiển như `Forward()`, `Rotate()` và các route phụ thuộc tốc độ, đường kính bánh, độ bám và kích thước sa bàn; cần hiệu chỉnh trên phần cứng thực tế.

## 8. Nguồn mã

- [Leanbot 1 – Phát hiện màu và ghi IoT](./iot1.ino)
- [Leanbot 2 – Đọc IoT và vận chuyển khối](./iot2.ino)
- [Thư mục Leanbot IoT](./)

> README mô tả hành vi **đang có trong hai file `.ino`**, không tự động thay đổi chương trình điều khiển hoặc bổ sung cơ chế đồng bộ/an toàn chưa được triển khai.
