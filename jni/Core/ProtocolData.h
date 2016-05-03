#ifndef CProtocolData_H
#define CProtocolData_H


#include "Core.h"
#include "tinystr.h"
#include "tinyxml.h"
#include <list>
#include "Data.h"


namespace CORE {


class CProtocolData;
class CORE_API CXMLHEAD
{
public:
	CXMLHEAD();
	~CXMLHEAD();
	CData MessageType();
	int MessageType(char *value);
	int GetSeq();
	int SetSeq(int seqNum);
	unsigned int GetSessionId();
	int SetSessionId(unsigned int sessionId);
	CData GetFrom();
	int SetFrom(char *value);
	CData GetEleType();
	int SetEleType(char *value);
	CData GetTo();
	int SetTo(char *value);

	void SetImp(CProtocolData* pData);
private:
	CProtocolData *m_pdata;
};

// �༶Ԫ�طָ���
const char NODE_DIVISION[3] = "/";
/**
 * @class CElement
 * @brief XML�ĵ���һ��Ԫ�صļ򵥷�װ
 * 
 * @n �������ֻ�ṩ�˻�ȡ������Ԫ�����ԡ�Ԫ��ֵ�ķ����Լ��򵥵ĺͿ�ָ
 * @n ��Ƚϵķ�����������CProtocolData��GetElement()������AddElement()
 * @n �����ķ���ֵ������ͨ����0�Ƚ��ж��Ƿ���ڸ�Ԫ�أ����磺
 * @n CProtocolData data;
 * @n data.Parse("test.xml");
 * @n CElement element = data.GetElement("Message/Header");
 * @n if (element!=0)
 * @n {
 * @n	element.SetAttribute("name","test");
 * @n	element.GetAttribute("name");
 * @n	...
 * @n }
 * @n CElement IE_result = data.AddElement("Message/IE_RESULT");
 * @n if (IE_result!=0)
 * @n {
 * @n	IE_result.SetAttribute("ErrorCode","10020");
 * @n	...
 * @n }
 * @n ͬʱ��CProtocolData�Ǹ������Ԫ�࣬CProtocolData�ķ����п���ֱ�ӷ�
 * @n ����˽�г�Աm_DOMElement���Է������
 * @n ע�⣺������Ƿ���XML���ݵĸ����ֻ࣬����CProtocolData�໹û������
 * @n ʱ������Ч�����CProtocolData�����������ʻ�Ƿ�����
 *
 */
class CORE_API CElement
{
	friend class CProtocolData;
public:
	// Ĭ�Ϲ��캯��m_DOMElement = 0,m_doc = 0;
	CElement();
	// ���캯��
	CElement(TiXmlElement* element,TiXmlDocument* doc = 0);
	// ��������
	~CElement();
	// �������캯��������m_DOMElementָ��ֵ
	CElement(const CElement& element);
	// ����Ԫ�����ԣ���������Ѵ��ڣ��滻����ֵ���������һ���µ�����
	int   SetAttribute(const char* name,const char* value);
	int   SetAttribute(const char* name,int value);
	// ����Ԫ��ֵ��0:success -1:error
	int   SetElementText(const char* text);
	int   SetElementText(int nText);
	// ��ȡԪ������ֵ�������ڻ�ʧ��ʱ����""
	CData GetAttribute(const char* name);
	// ��ȡ����ֵ��Ԫ��Ϊ�ջ��ȡʧ��ʱ����ֵΪ""
	CData GetElementText();
	// �жϸ�Ԫ���Ƿ�Ϊ�պ��ж��Ƿ�==0��ͬ
	bool isEmpty();
	// �ж��Ƿ���ȣ���Ҫ���ں�0�Ƚ��ж��Ƿ�Ϊ��
	bool operator == (TiXmlElement* element);
	// �ж��Ƿ���ȣ���Ҫ���ں�0�Ƚ��ж��Ƿ�Ϊ��
	bool operator != (TiXmlElement* element);
	CElement GetSubElement(const char* name,int index=0);
	CElement AddSubElement(const char* name);

	// ɾ����Ԫ��
	int RemoveSubElement(const char* name,int index = 0);
	
private:

	TiXmlElement*  m_DOMElement;
	TiXmlDocument* m_doc;
	int		  m_index;
};


/**
 * @class CProtocolData
 * @brief XMLЭ�����ݽ���������
 *
 * @n ���ļ������˷���ϵͳЭ��������XML��ʽ���ݵĽӿڷ���
 * @n ��XML���ݲ���DOM������������Parse()������������XML���ݣ�
 * @n ��������Ȼ˵ʹ�ö���GetValue������ȡ����Ҫ�����ݡ�
 * @li GetValue()����ֱ�ӻ�ȡXML�����е�ĳ��Ԫ�أ���������˶��
 * @n ������Ԫ��,ֻ�ܻ�ȡ��һ��Ԫ�ص����Ժ�ֵ
 * @li GetValueStep()���ڻ�ȡָ��Ԫ�ص����Ժ�ֵ�������ɶ��Ԫ��
 * @n ���ƺ�"/"��ɣ����磺Message/IE_Result
 * @n �����Ҫ���ʶ����ͬ��Ԫ�أ�ʹ��Query()�Ȳ�ѯ��Ȼ�����
 * @n Next()�������Ԫ�أ����磺
 * @n <?xml version="1.0" encoding="GB2312"?>
 * @n <Message>
 * @n	 <Test name="t1"/>
 * @n	 <Test name="t2"/>
 * @n </Message>
 * @n test.xml�ļ��е�����������ʾ
 * @n CProtocolData xmlData;
 * @n xmlData.Parse("test.xml"); 
 * @n CElement element = xmlData.Next();
 * @n while(element!=0)
 * @n {
 * @n	element.GetAttribute("name");
 * @n }
 */
class CORE_API CProtocolData  
{
	//˽�п������캯������֧�ֿ�������
	CProtocolData(CProtocolData& proData);
public:
	// ���캯��
	CProtocolData();
	// ��������
	virtual ~CProtocolData();

	//��ȡЭ��� char ��
	CData GetMessageType();
	int SetMessageType(const char *strType);  //����Э���
	/**
	 * @brief ���벢����XML����
	 * 
	 * @param[in] proData xml����
	 * @param[in] dataLen ���ݳ���
	 * @return 0  for success
	 * @return -1 for error,����������߽���ʧ��,�����־��Ϣ�쿴����
	 */
	int Parse(const char* proData,int dataLen);
	/**
	 * @brief ����XML�ļ�
	 *
	 * @param[in] fileName �ļ���
	 * @return 0  for success
	 * @return -1 for error,����������߽���ʧ��,�����־��Ϣ�쿴����
	 */
	int Parse(const char* fileName);

	/**
	 * @brief ֱ�ӻ�ȡԪ������
	 *
	 * ���ϵͳ���ж����ͬ��Ԫ�أ��ú���ֻ��ȡ��һ�������磺
	 * @n <?xml version="1.0" encoding="GB2312" ?>
	 * @n <Message>
	 * @n   <IE><Test name="t1"/></IE>
	 * @n   <Test name="t2"/>
	 * @n </Message>
	 * @n �������������GetValue("Test","name")=="t1"
	 * 
	 * @param[in] elementName Ԫ������
	 * @param[in] attributeName ��������
	 * @return CData ����ֵ�����û�л�ȡ��������""
	 */
	CData GetValue(const char* elementName,
				   const char* attributeName);
	/**
	 * @brief ֱ�ӻ�ȡԪ��ֵ
	 * 
	 * �ͻ�ȡԪ�����Ժ���һ����ֻ�ܻ�ȡ��һ��Ԫ�ص�ֵ��
	 * ����Э������û�ж���DTD,��ȡ��Ԫ��ֵ������ո�
	 * ��Ҫ������Э��ʱ��֤�����пո񣬷������׳������磺
	 * @n <?xml version="1.0" encoding="GB2312" ?>
	 * @n <Message>
	 * @n  <IE>fdsfs</IE>
	 * @n  <IE>fdsfs
	 * @n  </IE>
	 * @n </Message>
	 * �ڻ�ȡ��һ��IEԪ��ֵʱΪ"fdsfs"����ȡ�ڶ���IEԪ��ֵ
	 * ʱ������fdsfs������пո��ַ�(����' ',�س�����,Tab)
	 * 
	 * @param[in] elementName Ԫ������
	 * @return CData Ԫ��ֵ�����û�л�ȡ��������""
	 */
	CData GetValue(const char* elementName);

	/**
	 * @brief ��ȡԪ�����ԣ��𼶻�ȡ
	 *
	 * ��ȡָ��Ԫ�����ԣ�ͬGetValue����һ��������ж����
	 * ֻ�ܻ�ȡ��һ��Ԫ�أ����XML������û����ͬ��Ԫ�أ���
	 * ʹ��GetValue()ֱ�ӻ�ȡ�Ͽ죬���磺
	 * @n <?xml version="1.0" encoding="GB2312" ?>
	 * @n  <Message>
	 * @n	<IE name="test">fdsfs</IE>
	 * @n	<IE>fdsfs d</IE>
	 * @n  </Message>
	 * @n  GetValueStep("Message/IE","name")=="test"
	 * 
	 * @param[in] elementNames �༶Ԫ�����ƣ�name1/name2
	 * @param[in] attributeName ��������
	 * @return CData ����ֵ�����û�л�ȡ��������""
	 */
	CData GetValueStep(const char* elementNames,
					   const char* attributeName);
	/**
	 * @brief ��ȡԪ����ֵ���𼶻�ȡ
	 *
	 * ��ȡָ��Ԫ����ֵ��ͬGetValue����һ��������ж����
	 * ֻ�ܻ�ȡ��һ��Ԫ�أ����XML������û����ͬ��Ԫ�أ���
	 * ʹ��GetValue()ֱ�ӻ�ȡ�Ͽ죬���磺
	 * @n  <?xml version="1.0" encoding="GB2312" ?>
	 * @n  <Message>
	 * @n	<IE name="test">fdsfs</IE>
	 * @n	<IE>fdsfs d</IE>
	 * @n  </Message>
	 * @n  GetValueStep("Message/IE")=="fdsfs"
	 * 
	 * @param[in] elementNames �༶Ԫ�����ƣ�name1/name2
	 * @return CData Ԫ��ֵ�����û�л�ȡ��������""
	 */
	CData GetValueStep(const char* elementNames);

	/**
	 * @brief ��ѯ���Ԫ��
	 *
	 * ʹ��Query���Բ�ѯ���Ԫ�أ����elementNames���Ƕ༶��ʽ
	 * �������ĵ��в��ң����ָ����ĳ��Ԫ��(���༶���Ʒ�ʽ)��ֻ
	 * ����ָ���Ķ��Ԫ�ء���ѯ��ʹ��Next()���ʲ�ѯ��Ԫ�أ���ִ��
	 * �²�ѯǰ������ȵ���Reset()���������磺
	 * CProtocolData proData;
	 * proData.parse("XML����",���ݳ���);
	 * proData.Reset();
	 * proData.Query("IE");
	 * CElement element = proData.Next();
	 * while (element!=0)
	 * {
	 *	CData value = element.GetAttribute("attributeName");
	 *	// do something
	 * }
	 * 
	 * @param[in] elementNames Ԫ������
	 * @return >0 ��ѯ����Ԫ������
	 * @return -1 for error,
	 */
	int   Query(const char* elementNames);
	/**
	 * @brief ��һ��Ԫ��
	 *
	 * Query()��ʹ��Next()������ʲ�ѯ����Ԫ��
	 * 
	 * @return !=0 ��һ��Ԫ��
	 * @return ==0  ���ʽ���
	 */
	CElement Next();
	/**
	 * @brief ���ò�ѯ
	 *
	 * ��ִ��һ����ѯ����ִ���µĲ�ѯǰ��Ҫ���ø÷��������ò�ѯ����
	 * �Ͳ�ѯ���
	 * 
	 * @return void
	 */
	void  Reset();

	// ����һ���µ�Ԫ�أ��������ʧ�ܣ�����0
	CElement AddElement(const char* elementNames);
	CElement AddElementStep(const char* elementNames);
	/**
	 * @brief ����Ԫ������
	 * 
	 * @param[in] elementNames Ϊ�༶Ԫ�ط�ʽ�����磺Message/IE
	 * @param[in] name ��������
	 * @param[in] value ����ֵ
	 * @return 0  for success
	 * @return -1 for error,
	 */
	int SetAttribute(const char* elementNames,
					 const char* name,
					 const char* value);
	int SetAttribute(const char* elementNames, 
					 const char* name, 
					 int  value);
	// ����Ա��ֵ����SetAttribute��ͬelementNamesΪ�༶Ԫ�ط�ʽ
	int SetText(const char* elementNames,const char* text);
	int SetText(const char* elementNames,int nText);
	/**
	 * @brief ��ȡһ��Ԫ��
	 *
	 * ����ж����ͬ��Ԫ�أ�ֻ��ȡ��һ��
	 * 
	 * @param[in] elementNamesΪ�༶Ԫ�ط�ʽ
	 * @return !=0  ��ȡ�ɹ�
	 * @return ==0  δ��ȡ��
	 */
	CElement GetElement(const char* elementNames);
	/**
	 * @brief ��ȡXML�ĵ��Ķ�������ʽ������
	 *
	 * @param[out] data����ȡ�������� 
	 * @return 0  for success
	 * @return -1 for error,
	 */
	int GetMemoryBufferData(CData &data);

	bool GetProtocolStr(char *out, int *outLen);
	
	/**
	 * @brief ��XML�ĵ�д���ļ�
	 *
	 * @param[int] pFileName���ļ��� 
	 * @return 0  for success
	 * @return -1 for error,
	 */
	int WritetoFile(const char* pFileName);
	
	///���heard����
	CXMLHEAD *GetXmlHead();

private:

	TiXmlElement*  GetSubDOMElement(const TiXmlElement* parent,const char* elementName);
	
	TiXmlElement*  GetDOMElement(const char* elementName);
	
	TiXmlElement*  GetDOMElementStep(const char* multiElements);
	
	int GetDOMElementList(const char* elementNames);
	
	int GetElementsByName(TiXmlElement* element, const char* elementName);
	
private:

	TiXmlDocument* m_tixmlDom;
	int		  m_index;
	bool m_bParseSucc;
	CXMLHEAD m_xmlHead;
	std::list<TiXmlElement*> m_elementList;

};


}


#endif 


