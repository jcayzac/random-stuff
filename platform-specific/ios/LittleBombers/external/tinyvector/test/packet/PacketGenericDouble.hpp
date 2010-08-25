#include <QtTest/QtTest>

#define TV_DONT_VECTORIZE
#include "tv/packet"

class TestPacketGenericDouble : public QObject {

	Q_OBJECT

	private slots:
		void classMethods() {
			TV::packet::v2d v1, v2, v3(2.);
			
			for (unsigned int i = 0; i < 2; ++i) {
				v1[i] = 1.*qrand()/qrand();
				v2[i] = 1.*qrand()/qrand();
			}
			
			QVERIFY(v3[0] == 2.);
			QVERIFY(v3[1] == 2.);
			
			v1 = v2;
			QVERIFY(v1[0] == v2[0]);
			QVERIFY(v1[1] == v2[1]);
		}
		
		void simpleExpression() {
			TV::packet::v2d v1, v2, v3;
			
			for (unsigned int i = 0; i < 2; ++i) {
				v1[i] = 1.*qrand()/qrand();
				v2[i] = 1.*qrand()/qrand();
			}
			
			v3 = v1 + v2;
			QVERIFY(v3[0] == (v1[0] + v2[0]));
			QVERIFY(v3[1] == (v1[1] + v2[1]));
			
			v3 = v1 - v2;
			QVERIFY(v3[0] == (v1[0] - v2[0]));
			QVERIFY(v3[1] == (v1[1] - v2[1]));
			
			v3 = v1 * v2;
			QVERIFY(v3[0] == (v1[0] * v2[0]));
			QVERIFY(v3[1] == (v1[1] * v2[1]));
			
			v3 = v1 / v2;
			QVERIFY(v3[0] == (v1[0] / v2[0]));
			QVERIFY(v3[1] == (v1[1] / v2[1]));
		}
		
		void complexExpression() {
			TV::packet::v2d v1, v2, v3, v4, v5;
			
			for (unsigned int i = 0; i < 2; ++i) {
				v1[i] = 1.*qrand()/qrand();
				v2[i] = 1.*qrand()/qrand();
				v3[i] = 1.*qrand()/qrand();
				v4[i] = 1.*qrand()/qrand();
			}
			
			v5 = v1 + (v2 + v3);
			QVERIFY(v5[0] == (v1[0] + v2[0] + v3[0]));
			QVERIFY(v5[1] == (v1[1] + v2[1] + v3[1]));
			
			v5 = (v1 + v2) + v3;
			QVERIFY(v5[0] == (v1[0] + v2[0] + v3[0]));
			QVERIFY(v5[1] == (v1[1] + v2[1] + v3[1]));
			
			v5 = (v1 + v2) + (v3 + v4);
			QVERIFY(v5[0] == (v1[0] + v2[0] + v3[0] + v4[0]));
			QVERIFY(v5[1] == (v1[1] + v2[1] + v3[1] + v4[1]));
			
			v5 = v1 - (v2 * v3);
			QVERIFY(v5[0] == (v1[0] - v2[0] * v3[0]));
			QVERIFY(v5[1] == (v1[1] - v2[1] * v3[1]));
			
			v5 = (v1 / v2) * v3;
			QVERIFY(v5[0] == (v1[0] / v2[0] * v3[0]));
			QVERIFY(v5[1] == (v1[1] / v2[1] * v3[1]));
			
			v5 = (v1 + v2) - (v3 * v4);
			QVERIFY(v5[0] == (v1[0] + v2[0] - v3[0] * v4[0]));
			QVERIFY(v5[1] == (v1[1] + v2[1] - v3[1] * v4[1]));
		}
};
