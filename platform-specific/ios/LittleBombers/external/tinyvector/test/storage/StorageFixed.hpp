#include <QtTest/QtTest>

#include "tv/storage"

class TestStorageFixed : public QObject {

	Q_OBJECT
	
	private slots:
		void constructorMethods() {
			TV::storage::fixed<float, 10> s1(2.);
			TV::storage::fixed<float, 10> s2(s1);
			
			for (unsigned int i = 0; i < 10; ++i)
				QVERIFY(s1(i) == s2(i));
		}
		
		void otherMethods() {
			TV::storage::fixed<float, 8> s1(2.);
			TV::storage::fixed<float, 8> s2;
			
			s1(4) = 1.;
			s1(5) = 1.;
			s1(6) = 1.;
			s1(7) = 1.;
			
			s2 = s1;
			
			for (unsigned int i = 0; i < 8; ++i)
				QVERIFY(s1(i) == s2(i));
			
			s2[0] = s1[1];
			s2[1] = s1[0];
			
			for (unsigned int i = 0; i < 8; ++i)
				QVERIFY(s1(i) == s2((i+4)%8));
		}

};
