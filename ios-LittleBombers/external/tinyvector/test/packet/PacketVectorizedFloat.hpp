#include <QtTest/QtTest>

#include "tv/packet"

class TestPacketVectorizedFloat : public QObject {

	Q_OBJECT

	private slots:
		void classMethods() {
			TV::packet::v4f v1, v2, v3(2.);
			
			for (unsigned int i = 0; i < 4; ++i) {
				v1[i] = 1.*qrand()/qrand();
				v2[i] = 1.*qrand()/qrand();
			}
			
			QVERIFY(v3[0] == 2.);
			QVERIFY(v3[1] == 2.);
			QVERIFY(v3[2] == 2.);
			QVERIFY(v3[3] == 2.);
			
			v1 = v2;
			QVERIFY(v1[0] == v2[0]);
			QVERIFY(v1[1] == v2[1]);
			QVERIFY(v1[2] == v2[2]);
			QVERIFY(v1[3] == v2[3]);
		}
		
		void simpleExpression() {
			TV::packet::v4f v1, v2, v3;
			
			for (unsigned int i = 0; i < 4; ++i) {
				v1[i] = 1.*qrand()/qrand();
				v2[i] = 1.*qrand()/qrand();
			}
			
			v3 = v1 + v2;
			QVERIFY(v3[0] == (v1[0] + v2[0]));
			QVERIFY(v3[1] == (v1[1] + v2[1]));
			QVERIFY(v3[2] == (v1[2] + v2[2]));
			QVERIFY(v3[3] == (v1[3] + v2[3]));
			
			v3 = v1 - v2;
			QVERIFY(v3[0] == (v1[0] - v2[0]));
			QVERIFY(v3[1] == (v1[1] - v2[1]));
			QVERIFY(v3[2] == (v1[2] - v2[2]));
			QVERIFY(v3[3] == (v1[3] - v2[3]));
			
			v3 = v1 * v2;
			QVERIFY(v3[0] == (v1[0] * v2[0]));
			QVERIFY(v3[1] == (v1[1] * v2[1]));
			QVERIFY(v3[2] == (v1[2] * v2[2]));
			QVERIFY(v3[3] == (v1[3] * v2[3]));
			
			v3 = v1 / v2;
			QVERIFY(v3[0] == (v1[0] / v2[0]));
			QVERIFY(v3[1] == (v1[1] / v2[1]));
			QVERIFY(v3[2] == (v1[2] / v2[2]));
			QVERIFY(v3[3] == (v1[3] / v2[3]));
		}
		
		void complexExpression() {
			TV::packet::v4f v1, v2, v3, v4, v5;
			
			for (unsigned int i = 0; i < 4; ++i) {
				v1[i] = 1.*qrand()/qrand();
				v2[i] = 1.*qrand()/qrand();
				v3[i] = 1.*qrand()/qrand();
				v4[i] = 1.*qrand()/qrand();
			}
			
			v5 = v1 + (v2 + v3);
			QVERIFY(v5[0] == (v1[0] + v2[0] + v3[0]));
			QVERIFY(v5[1] == (v1[1] + v2[1] + v3[1]));
			QVERIFY(v5[2] == (v1[2] + v2[2] + v3[2]));
			QVERIFY(v5[3] == (v1[3] + v2[3] + v3[3]));
			
			v5 = (v1 + v2) + v3;
			QVERIFY(v5[0] == (v1[0] + v2[0] + v3[0]));
			QVERIFY(v5[1] == (v1[1] + v2[1] + v3[1]));
			QVERIFY(v5[2] == (v1[2] + v2[2] + v3[2]));
			QVERIFY(v5[3] == (v1[3] + v2[3] + v3[3]));
			
			v5 = (v1 + v2) + (v3 + v4);
			QVERIFY(v5[0] == (v1[0] + v2[0] + v3[0] + v4[0]));
			QVERIFY(v5[1] == (v1[1] + v2[1] + v3[1] + v4[1]));
			QVERIFY(v5[2] == (v1[2] + v2[2] + v3[2] + v4[2]));
			QVERIFY(v5[3] == (v1[3] + v2[3] + v3[3] + v4[3]));
			
			v5 = v1 - (v2 * v3);
			QVERIFY(v5[0] == (v1[0] - v2[0] * v3[0]));
			QVERIFY(v5[1] == (v1[1] - v2[1] * v3[1]));
			QVERIFY(v5[2] == (v1[2] - v2[2] * v3[2]));
			QVERIFY(v5[3] == (v1[3] - v2[3] * v3[3]));
			
			v5 = (v1 / v2) * v3;
			QVERIFY(v5[0] == (v1[0] / v2[0] * v3[0]));
			QVERIFY(v5[1] == (v1[1] / v2[1] * v3[1]));
			QVERIFY(v5[2] == (v1[2] / v2[2] * v3[2]));
			QVERIFY(v5[3] == (v1[3] / v2[3] * v3[3]));
			
			v5 = (v1 + v2) - (v3 * v4);
			QVERIFY(v5[0] == (v1[0] + v2[0] - v3[0] * v4[0]));
			QVERIFY(v5[1] == (v1[1] + v2[1] - v3[1] * v4[1]));
			QVERIFY(v5[2] == (v1[2] + v2[2] - v3[2] * v4[2]));
			QVERIFY(v5[3] == (v1[3] + v2[3] - v3[3] * v4[3]));
		}
};
