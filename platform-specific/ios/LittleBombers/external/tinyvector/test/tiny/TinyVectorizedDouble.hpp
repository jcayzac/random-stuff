#include <QtTest/QtTest>

#include "tv/tiny"

class TestTinyVectorizedDouble : public QObject {
	
	Q_OBJECT
	
	private slots:
		void constructorMethods() {
			TV::tiny::vector<double, 12> v1(2.);
			TV::tiny::vector<double, 12> v2(v1);
			TV::tiny::vector<double, 12> v3(v1 * 2.e0);
			
			for (unsigned int i = 0; i < 12; ++i)
				QVERIFY(v2(i) == v1(i));
			for (unsigned int i = 0; i < 12; ++i)
				QVERIFY(v3(i) == v1(i) * 2.e0);
		}
		
		void expressionTemplates() {
			TV::tiny::vector<double, 12> v1, v2(2.), v3(4.), v4(6.), v5(8.);
			
			v1 = v2 * 2.e0;
			for(unsigned int i = 0; i < 12; ++i)
				QVERIFY(v1(i) == (v2(i) * 2.));
			
			v1 = v2 * 2.e0 + v3;
			for(unsigned int i = 0; i < 12; ++i)
				QVERIFY(v1(i) == (v2(i) * 2. + v3(i)));
			
			v1 = v2 * 2.e0 + v3 - v4 + v5 * 3.14e0;
			for(unsigned int i = 0; i < 12; ++i)
				QVERIFY(v1(i) == (v2(i) * 2. + v3(i) - v4(i) + v5(i) * 3.14e0));
		}
		
		void benchExpression() {
			TV::tiny::vector<double, 100> v1, v2(2.), v3(4.), v4(6.), v5(8.);
			
			QBENCHMARK {
				v1 = v2 * 2.e0;
				v1 = v2 * 2.e0 + v3;
				v1 = v2 * 2.e0 + v3 - v4;
				v1 = v2 * 2.e0 + v3 - v4 + v5 * 3.14e0;
			}
		}
};
