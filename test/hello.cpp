class NodeTest : public ::testing::Test {

}

TEST_F(NodeTest, EmptyTest) {
	ASSERT_TRUE(true);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
