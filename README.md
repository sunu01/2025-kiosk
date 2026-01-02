📟 Qt/QML 기반 실시간 주문 키오스크(POS) 시스템
📌 프로젝트 개요

본 프로젝트는 **Qt/QML과 C++**을 활용하여 구현한
실시간 주문 처리 키오스크(POS) 애플리케이션입니다.

외부 클라이언트로부터 **TCP 통신(JSON 형식)**을 통해 주문 데이터를 수신하고,
테이블 상태 및 주문 내역을 UI에 실시간으로 반영하도록 설계되었습니다.

또한 Qt SQL 모듈을 활용한 데이터베이스 연동 구조를 구성하여,
주문 데이터의 영속적 관리를 고려한 확장 가능한 구조로 구현되었습니다.

🎯 주요 기능

TCP 서버 기반 주문 수신

JSON 형식 주문 데이터 처리

테이블 상태 실시간 갱신

테이블별 주문 상세 화면

결제(테이블 초기화) 기능

DB 연동 구조 및 초기화 로직 구현

UI–비즈니스 로직 분리 설계

🧱 시스템 아키텍처
[Client]
   ↓ TCP(JSON)
EchoServer (QTcpServer)
   ↓
main.cpp (Routing)
   ↓
TableManager (Business Logic)
   ↓
QML UI (TablePage / TableDetailPage)

[Database]
   ↑
Qt SQL (Connection & Initialization)

🛠 기술 스택
Language & Framework

C++

Qt Framework

QML / Qt Quick Controls

Network

QTcpServer

QTcpSocket

TCP 기반 JSON 통신

Database

Qt SQL Module (QSqlDatabase)

DB 연결 및 초기화 구현

데이터 영속성 확장 고려 설계

Data Handling

QJsonDocument / QJsonObject

QVariantMap / QVariantList

UI Architecture

StackView 기반 화면 전환

Page / Item 구조

Model–View 분리

🔍 핵심 구현 내용
1️⃣ TCP 기반 실시간 주문 처리

외부 클라이언트와 TCP 연결

JSON 주문 데이터 파싱 및 검증

수신 즉시 테이블 상태 반영

2️⃣ 비즈니스 로직 중앙 관리

테이블 상태 및 주문 데이터는 TableManager에서 일괄 관리

UI는 상태 표시만 담당

유지보수 및 확장에 유리한 구조

3️⃣ 데이터베이스 연동 구조

애플리케이션 시작 시 DB 연결 초기화

DB 연결 실패 시 앱 실행 중단

주문 데이터 저장을 고려한 확장 구조

4️⃣ QML–C++ 데이터 바인딩

Q_PROPERTY를 활용한 실시간 UI 갱신

서버 주문 → UI 자동 반영

🧪 테스트 방법
서버 실행

애플리케이션 실행 후 서버를 활성화합니다.

주문 테스트 (PowerShell)
$client = New-Object System.Net.Sockets.TcpClient("127.0.0.1", 8080)
$stream = $client.GetStream()

$msg = '{"type":"order","tableId":1,"menu":"아메리카노","quantity":2,"price":8000}'
$bytes = [System.Text.Encoding]::UTF8.GetBytes($msg)

$stream.Write($bytes, 0, $bytes.Length)
$stream.Close()
$client.Close()

결과

테이블 화면에서 해당 테이블의 금액 표시

상세 페이지에서 주문 목록 실시간 갱신
