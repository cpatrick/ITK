/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    testMetaCommand.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <stdio.h>
#include <fstream>
#include <ctype.h>


#include <metaCommand.h>

int testMetaCommand(int argc, char * argv[])
{
  MetaCommand command;
  command.SetOption("ExpectedFailStatus","ExpectedFailStatus",true,"Use to check that proper failures are listed.");
  command.AddOptionField("ExpectedFailStatus","ExpectedFailStatus", MetaCommand::INT,true,"1","Expected fail status is true");

  command.SetOption("SumOfValues","sum",true,"Sum of values passed into command line, Default values are 100");
  command.AddOptionField("SumOfValues","SumOfValues", MetaCommand::INT,true);

  command.SetOption("Required1Complete_rs","r1c_rs",true, "RequiredOption1");
  command.AddOptionField("Required1Complete_rs","Required1Complete_rs", MetaCommand::STRING,true);
  command.SetOptionComplete("Required1Complete_rs",true);

  command.SetOption("Required2_ri","r2_ri",true,"Required2");
  command.AddOptionField("Required2_ri","Required2_ri", MetaCommand::INT,true);

  command.SetOption("OptionalField1_ri","o1_ri",false, "Optional Field 1");
  command.AddOptionField("OptionalField1_ri","OptionalField1_ri", MetaCommand::INT,true,"100","A value of 100");

  command.SetOption("OptionalField2_ri","o2_ri",false, "Optional Field 1");
  command.AddOptionField("OptionalField2_ri","OptionalField2_ri", MetaCommand::INT,true,"100","A value of 100");

  command.SetOption("OptionalField1_oi","o1_oi",false, "Optional Field 2");
  command.AddOptionField("OptionalField1_oi","OptionalField1_oi", MetaCommand::INT,false,"100","A value of 100");

  command.SetOption("OptionalField2_oi","o2_oi",false, "Optional Field 2");
  command.AddOptionField("OptionalField2_oi","OptionalField2_oi", MetaCommand::INT,false,"100","A value of 100");

  command.SetOption("OptionalField3_ri_oi","o3_ri_oi",false, "Optional Field 3, one required, one optional flag");
  command.AddOptionField("OptionalField3_ri_oi","OptionalField3_ri", MetaCommand::INT,true);
  command.AddOptionField("OptionalField3_ri_oi","OptionalField3_oi", MetaCommand::INT,false,"100","A value of 100");

  if(command.Parse(argc,argv) == false )
    {
    if(command.GetValueAsInt("ExpectedFailStatus","ExpectedFailStatus") == 1)
      {
      std::cout << "Expected parse failure, so test succeded" << std::endl;
      return 0;
      }
    else
      {
      std::cout << "Unexpected parse failure, so test failed" << std::endl;
      return 1;
      }
    }
  else
    {
    const int SumValue=
      +( command.GetOptionWasSet("Required2_ri")         ? command.GetValueAsInt("Required2_ri","Required2_ri") : 0 )
      +( command.GetOptionWasSet("OptionalField1_ri")    ? command.GetValueAsInt("OptionalField1_ri","OptionalField1_ri"): 0 )
      +( command.GetOptionWasSet("OptionalField2_ri")    ? command.GetValueAsInt("OptionalField2_ri","OptionalField2_ri"): 0 )
      +( command.GetOptionWasSet("OptionalField1_oi")    ? command.GetValueAsInt("OptionalField1_oi","OptionalField1_oi"): 0 )
      +( command.GetOptionWasSet("OptionalField2_oi")    ? command.GetValueAsInt("OptionalField2_oi","OptionalField2_oi"): 0 )
      +( command.GetOptionWasSet("OptionalField3_ri_oi") ? command.GetValueAsInt("OptionalField3_ri_oi","OptionalField3_ri"): 0 )
      +( command.GetOptionWasSet("OptionalField3_ri_oi") ? command.GetValueAsInt("OptionalField3_ri_oi","OptionalField3_oi"): 0 );
    std::cout << "Computed " <<  SumValue << " expected " << command.GetValueAsInt("SumOfValues") << std::endl;

    if(command.GetValueAsInt("ExpectedFailStatus","ExpectedFailStatus") == 1)
      {
      std::cout << "Expected parse failure that did not occur, so test failed" << std::endl;
      return 1;
      }
    else
      {
      return ( command.GetValueAsInt("SumOfValues") - SumValue );
      }
    }
  //Should never reach this point.
  return -1;
}
