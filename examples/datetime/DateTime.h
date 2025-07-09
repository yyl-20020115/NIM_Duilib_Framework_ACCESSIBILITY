#pragma once

namespace ui
{
    class DateTimeWnd;

    class DateTime : public ui::Label
    {

    public:
        friend class DateTimeWnd;
        DateTime();

        const SYSTEMTIME& GetTime() const;
        void SetTime(const SYSTEMTIME& val);

        void UpdateText();

        const std::wstring& GetFormat() const;
        void SetFormat(std::wstring val);

        const std::wstring& GetDateTimeFormat() const;
        void SetDateTimeFormat(std::wstring val);


        virtual void HandleMessage(EventArgs& msg) override;


        virtual void SetAttribute(const std::wstring& strName, const std::wstring& strValue) override;

    protected:
        SYSTEMTIME m_sysTime;
        int m_nDTUpdateFlag;

        /*
        %a ���ڼ��ļ�д
        %A ���ڼ���ȫ��
        %b �·ֵļ�д
        %B �·ݵ�ȫ��
        %c ��׼�����ڵ�ʱ�䴮
        %C ��ݵĺ���λ����
        %d ʮ���Ʊ�ʾ��ÿ�µĵڼ���
        %D ��/��/��
        %e �����ַ����У�ʮ���Ʊ�ʾ��ÿ�µĵڼ���
        %F ��-��-��
        %g ��ݵĺ���λ���֣�ʹ�û����ܵ���
        %G ��֣�ʹ�û����ܵ���
        %h ��д���·���
        %H 24Сʱ�Ƶ�Сʱ
        %I 12Сʱ�Ƶ�Сʱ
        %j ʮ���Ʊ�ʾ��ÿ��ĵڼ���
        %m ʮ���Ʊ�ʾ���·�
        %M ʮʱ�Ʊ�ʾ�ķ�����
        %n ���з�
        %p ���ص�AM��PM�ĵȼ���ʾ
        %r 12Сʱ��ʱ��
        %R ��ʾСʱ�ͷ��ӣ�hh:mm
        %S ʮ���Ƶ�����
        %t ˮƽ�Ʊ��
        %T ��ʾʱ���룺hh:mm:ss
        %u ÿ�ܵĵڼ��죬����һΪ��һ�� ��ֵ��0��6������һΪ0��
        %U ����ĵڼ��ܣ�����������Ϊ��һ�죨ֵ��0��53��
        %V ÿ��ĵڼ��ܣ�ʹ�û����ܵ���
        %w ʮ���Ʊ�ʾ�����ڼ���ֵ��0��6��������Ϊ0��
        %W ÿ��ĵڼ��ܣ�������һ��Ϊ��һ�죨ֵ��0��53��
        %x ��׼�����ڴ�
        %X ��׼��ʱ�䴮
        %y �������͵�ʮ������ݣ�ֵ��0��99��
        %Y �����Ͳ��ֵ�ʮ�������
        %z��%Z ʱ�����ƣ�������ܵõ�ʱ�������򷵻ؿ��ַ���
        %% �ٷֺ�
        */
        std::wstring m_sFormat;

        /*
        �� dd����λ�������ڡ�һλ���ֵ�����ֵǰ����һ���㡣
        �� ddd���������ַ��Ĺ�������д��
        �� dddd�������Ĺ��������ơ�
        �� h�� 12Сʱ��ʽ��һλ����λ��Сʱ��
        �� hh�� 12Сʱ��ʽ����λ��Сʱ��һλ��ֵǰ���㡣
        �� H�� 24Сʱ��ʽ��һλ����λ��Сʱ��
        �� HH�� 24Сʱ��ʽ����λ��Сʱ��һλ��ֵǰ���㡣
        �� m��һλ����λ���ַ��ӡ�
        �� mm����λ�����ӡ�һλ��ֵǰ���㡣
        �� M��һλ����λ�����·ݺš�
        �� MM����λ�����·ݺš�һλ��ֵǰ���㡣
        �� MMM���������ַ����·���д��
        �� MMMM���������·����ơ�
        �� t�� AM / PMһ����ĸ����д����AM��ʾΪ�� A������
        �� tt��������ĸ��AM / PM��д����AM��ʾΪ�� AM������
        �� yy����ݵ������λ���֣���1996����ʾΪ�� 96������
        �� yyyy��ȫ�꣨��1996�꽫��ʾΪ�� 1996������
        */
        std::wstring m_sDateTimeFormat;

        DateTimeWnd* m_pDateTimeWnd;
    };
}

