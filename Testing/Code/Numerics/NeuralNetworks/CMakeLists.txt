INCLUDE_REGULAR_EXPRESSION("^.*$")

IF( NOT ITK_DISABLE_CXX_TESTING )
 IF(ITK_BINARY_DIR)
  IF(TCL_TCLSH)
    ADD_TEST(PrintSelf-NeuralNets ${TCL_TCLSH}
         ${ITK_SOURCE_DIR}/Utilities/Dart/PrintSelfCheck.tcl  
             ${ITK_SOURCE_DIR}/Code/Numerics/NeuralNetworks)
  ENDIF(TCL_TCLSH)
ENDIF(ITK_BINARY_DIR)

  SET(NEURAL_NETWORKS_HEADER_TEST  ${CXX_TEST_PATH}/itkNeuralNetworksHeaderTest)
    
   
  LINK_DIRECTORIES(
      ${ITK_BINARY_DIR}/Code/Numerics/NeuralNetworks 
    )
    
   LINK_LIBRARIES(
      ${ITK_LIBRARIES}
    )
    
 
  SET(NNET_OBJECT_TESTS  ${CXX_TEST_PATH}/itkNeuralNetworkTests)
  SET(NNET_OBJECT_TESTS2 ${CXX_TEST_PATH}/itkNeuralNetworkTests2)
  SET(NNET_OBJECT_TESTS3 ${CXX_TEST_PATH}/itkNeuralNetworkTests3)

  ADD_TEST( itkNeuralNetworksHeaderTest ${NEURAL_NETWORKS_HEADER_TEST})

  ADD_TEST(QPropXORTest1 ${NNET_OBJECT_TESTS} 
            QPropXORTest1   ${ITK_DATA_ROOT}/Input/NeuralNetworks/qpropxortest.txt)
  ADD_TEST(XORTest1 ${NNET_OBJECT_TESTS} 
            XORTest1  ${ITK_DATA_ROOT}/Input/NeuralNetworks/xortest.txt)
  ADD_TEST(XORTest2 ${NNET_OBJECT_TESTS} 
            XORTest2   ${ITK_DATA_ROOT}/Input/NeuralNetworks/xortest.txt)
  ADD_TEST(NNetClassifierTest1 ${NNET_OBJECT_TESTS3} 
           NNetClassifierTest1 ${ITK_DATA_ROOT}/Input/NeuralNetworks/train.txt 
                               ${ITK_DATA_ROOT}/Input/NeuralNetworks/test.txt)
  ADD_TEST(NNetClassifierTest2 ${NNET_OBJECT_TESTS3} 
           NNetClassifierTest2 ${ITK_DATA_ROOT}/Input/NeuralNetworks/train.txt 
                                 ${ITK_DATA_ROOT}/Input/NeuralNetworks/test.txt)
  ADD_TEST(NNetClassifierTest3 ${NNET_OBJECT_TESTS3} 
           NNetClassifierTest3 ${ITK_DATA_ROOT}/Input/NeuralNetworks/train.txt 
                               ${ITK_DATA_ROOT}/Input/NeuralNetworks/test.txt)
  ADD_TEST(NNetClassifierTest4 ${NNET_OBJECT_TESTS3} 
           NNetClassifierTest4 ${ITK_DATA_ROOT}/Input/NeuralNetworks/train.txt 
                               ${ITK_DATA_ROOT}/Input/NeuralNetworks/test.txt)
  ADD_TEST(RBFTest1 ${NNET_OBJECT_TESTS2} 
           RBFTest1 ${ITK_DATA_ROOT}/Input/NeuralNetworks/rbf_multivar_gauss_train.txt
                    ${ITK_DATA_ROOT}/Input/NeuralNetworks/rbf_multivar_gauss_test.txt)
  
  ADD_TEST( itkNeuralNetworksPrintTest ${NNET_OBJECT_TESTS2} itkNeuralNetworksPrintTest)

   SET(NNET_SRCS
      XORTest1.cxx
      XORTest2.cxx
      QPropXORTest1.cxx
    )

  SET(NNET_SRCS2
      RBFTest1.cxx
      itkNeuralNetworksPrintTest.cxx
    )

   SET(NNET_SRCS3
      NNetClassifierTest1.cxx
      NNetClassifierTest2.cxx
      NNetClassifierTest3.cxx
      NNetClassifierTest4.cxx
    )

  

  ADD_EXECUTABLE(itkNeuralNetworkTests itkNeuralNetworkTests.cxx ${NNET_SRCS})
  TARGET_LINK_LIBRARIES(itkNeuralNetworkTests ITKIO ITKNumerics ITKStatistics)

  ADD_EXECUTABLE(itkNeuralNetworkTests2 itkNeuralNetworkTests2.cxx ${NNET_SRCS2})
  TARGET_LINK_LIBRARIES(itkNeuralNetworkTests2 ITKIO ITKNumerics ITKStatistics)

  ADD_EXECUTABLE(itkNeuralNetworkTests3 itkNeuralNetworkTests3.cxx ${NNET_SRCS3})
  TARGET_LINK_LIBRARIES(itkNeuralNetworkTests3 ITKIO ITKNumerics ITKStatistics)

  ADD_EXECUTABLE(itkNeuralNetworksHeaderTest itkNeuralNetworksHeaderTest.cxx)
  TARGET_LINK_LIBRARIES(itkNeuralNetworksHeaderTest ITKIO ITKNumerics ITKStatistics)

  IF(ITK_OBJCXX_COMPILER_WORKS)
    SET(NEURALNETWORKS_OBJCXX_TEST ${CXX_TEST_PATH}/itkNeuralNetworksHeaderObjCxxTest)
    ADD_TEST(itkNeuralNetworksHeaderObjCxxTest ${NEURALNETWORKS_OBJCXX_TEST})
    ADD_EXECUTABLE(itkNeuralNetworksHeaderObjCxxTest itkNeuralNetworksHeaderObjCxxTest.mm)
    TARGET_LINK_LIBRARIES(itkNeuralNetworksHeaderObjCxxTest ITKIO ITKNumerics ITKStatistics)
  ENDIF(ITK_OBJCXX_COMPILER_WORKS)

ENDIF( NOT ITK_DISABLE_CXX_TESTING )
