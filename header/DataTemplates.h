#include<QDate>
#include<QList>
#include<QString>
#include<TypeCode.h>
struct BookLocation {
	Library libraryID = Library::Illegal; // 馆
	int floor = -1;                       // 楼层
	Area areaID = Area::Illegal;          // 图书区
	int shelf = -1;                       // 架
	int layer = -1;                       // 层
	//有效性自检
	bool isValid() const {
		return libraryID != Library::Illegal &&
			areaID != Area::Illegal &&
			floor >= 0 && shelf >= 0 && layer >= 0;
	}
};//馆藏位置
class ISBN {
private:
	QString Value;//ISBN的值
	bool isValid;
public:
	// 缺省构造：处于非法空状态
	ISBN() : Value(""), isValid(false) {}
	// Setter
	[[nodiscard]] ErrorCode SetValue(const QString& Input) {
		//空输入检查
		if (Input.isEmpty()) {
			isValid = false;
			return ErrorCode::EMPTY_INPUT;
		}
		//正负检查
		if (Input.startsWith('-')) {
			isValid = false;
			return ErrorCode::ILLEGAL_INPUT;
		}
		// 位数检查，仅放行10位或13位的纯数字串
		int len = Input.length();
		if (len == 10 || len == 13) {
			Value = Input;
			isValid = true;
			return ErrorCode::SUCCESS;
		}
		else {
			isValid = false;
			return ErrorCode::ILLEGAL_INPUT;
		}//位数不符
	}
	// Getter
	const QString& qs_Value() const { return Value; }
	const bool b_isValid() const { return isValid; }
};
class Volume {
private:
	long long int ID;//单册ID，默认初始化置零，实际数据应为条码编号
	Availability IsAvailable;//可用状态
	bool IsOpenshelf;//是否开架图书，1为开架图书，0为闭架图书
	BookLocation Location;//馆藏位置
	QDate DueDate;//外借到期时间
	long long int BorrowerID;//目前借阅者的ID，预备供用户管理使用
public:
	Volume(long long int a = 0, Availability b = Availability::Illegal, bool c = 0, QDate date=QDate()) {
		ID = a; IsAvailable = b; IsOpenshelf = c;
		DueDate = date;
		BorrowerID = -1;
	}
	const long long int lli_ID() const { return ID; }
	const Availability i_IsAvailable() const { return IsAvailable; }
	const bool b_IsOpenshelf() const { return IsOpenshelf; }
	const BookLocation& stct_Location() const { return Location; }
	const QDate& qd_DueDate() const { return DueDate; }
	ErrorCode SetID(const long long int in) {
		if (in > 0) { ID = in; return ErrorCode::SUCCESS; }
		else  return ErrorCode::ILLEGAL_INPUT;
	}
	ErrorCode SetBorrwerID(const long long int id) {
		if (IsAvailable == Availability::Available) { BorrowerID = id; return ErrorCode::SUCCESS; }
		else  return ErrorCode::ILLEGAL_INPUT;
	}
	ErrorCode SetAvailability(const Availability in) {
		if (in != Availability::Illegal) { IsAvailable = in; return ErrorCode::SUCCESS; }
		else  return ErrorCode::ILLEGAL_INPUT;
	}
	void SetDueDate(const QDate& in) { DueDate = in; }
	void SetLocation(const BookLocation& in) { Location = in;	}
};
class Book{
private:
	ISBN BookISBN;//ISBN编码
	QString Name;//书名
	QList<QString> Author;//作者（一位或若干位）
	QString Press;//出版社
	Category PubCategory;//图书分类
	int PubYear;//出版年份
	Language PubLanguage;//出版文种
	QList<Volume> VolumeList;//单册列表
public:
	Book( QString b = "", QList<QString> auth = QList<QString>(), QString prss = "",
		Category cat = Category::Illegal, int yr = -9999, Language lang = Language::Illegal,
		QList<Volume> list = QList<Volume>()) {
		Name = b; Author = auth; Press = prss;
		PubCategory = cat; PubYear = yr; PubLanguage = lang;
		VolumeList = list;
	}
	const QString& qs_Name() const { return Name; }
	const QList<QString>& ql_Author() const { return Author; }
	const QString& qs_Press() const { return Press; }
	const Category cat_PubCategory() const { return PubCategory; }
	const int i_PubYear() const { return PubYear; }
	const Language lang_PubLanguage() const { return PubLanguage; }
	const QList<Volume>& ql_VolumeList() const { return VolumeList; }
	ErrorCode SetISBN(const QString& in) { return BookISBN.SetValue(in); }
	ErrorCode SetName(const QString& in) {
		if (in.isEmpty()) {  return ErrorCode::EMPTY_INPUT; }
		else { Name = in; return ErrorCode::SUCCESS; }
	}
	ErrorCode SetAuthor(const QList<QString>& in) {
		if (in.isEmpty()||in[0].isEmpty()) { return ErrorCode::EMPTY_INPUT; }
		else { Author = in; return ErrorCode::SUCCESS; }
	}
	ErrorCode SetPress(const QString& in) {
		if (in.isEmpty()) { return ErrorCode::EMPTY_INPUT; }
		else { Press = in; return ErrorCode::SUCCESS; }
	}
	ErrorCode SetPubCategory(const Category in) {
		if (in != Category::Illegal) { PubCategory = in; return ErrorCode::SUCCESS; }
		else  return ErrorCode::ILLEGAL_INPUT;
	}
	void SetPubYear(const int yr) { PubYear = yr; }
	ErrorCode SetPubLanguage(const Language in) {
		if (in != Language::Illegal) { PubLanguage = in; return ErrorCode::SUCCESS; }
		else  return ErrorCode::ILLEGAL_INPUT;
	}
};
class LoanRecord {
private:
	long long int BorrowerID;//借阅者ID
	bool IsReturned;//是否已归还，默认为1（已归还）
	QDate LoanDate;//借书日期
	QDate DueDate;//应还日期
public:
	LoanRecord(long long int a = -1, bool b = true, QDate c = QDate(), QDate d = QDate()) :
		BorrowerID(a), IsReturned(b), LoanDate(c), DueDate(d) {	}
	const bool b_IsReturned() const { return IsReturned; }
	const QDate qd_LoanDate() const { return LoanDate; }
	const QDate qd_DueDate() const { return DueDate; }
};
class BorrowHistory {
private:
	long long int ISBN;
	QList<LoanRecord> RecordList;
public:
	BorrowHistory(long long int a=-1,QList<LoanRecord> b=QList<LoanRecord>()){
		ISBN = a; RecordList = b;
	}
	const QList<LoanRecord> ql_RecordList() const { return RecordList; }
};

