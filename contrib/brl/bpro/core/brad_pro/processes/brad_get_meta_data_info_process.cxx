//This is brl/bpro/core/brad_pro/processes/brad_get_sun_angles_process.cxx
//:
// \file
//     extract the sun azimuth and elevation angles from an image_metadata object
//
//

#include <bprb/bprb_func_process.h>

#include <brad/brad_image_metadata.h>

//: set input and output types
bool brad_get_meta_data_info_process_cons(bprb_func_process& pro)
{
  vcl_vector<vcl_string> input_types;
  input_types.push_back("brad_image_metadata_sptr"); // image name

  if (!pro.set_input_types(input_types))
    return false;

  vcl_vector<vcl_string> output_types;
  output_types.push_back("float"); // sun azimuth angle
  output_types.push_back("float"); // sun elevation
  output_types.push_back("int"); // year
  output_types.push_back("int"); // month
  output_types.push_back("int"); // day
  output_types.push_back("int"); // hour
  output_types.push_back("int"); // minute
  output_types.push_back("int"); // seconds
  output_types.push_back("float"); // ground sampling distance (GSD)
  output_types.push_back("vcl_string"); // satellite name
  return pro.set_output_types(output_types);
}

bool brad_get_meta_data_info_process(bprb_func_process& pro)
{
  if ( pro.n_inputs() != pro.input_types().size() )
  {
    vcl_cout << pro.name() << " The number of inputs should be " << pro.input_types().size() << vcl_endl;
    return false;
  }

  //get the inputs
  brad_image_metadata_sptr mdata = pro.get_input<brad_image_metadata_sptr>(0);

  pro.set_output_val<float>(0, float(mdata->sun_azimuth_));
  pro.set_output_val<float>(1, float(mdata->sun_elevation_));
  pro.set_output_val<int>(2, mdata->t_.year);
  pro.set_output_val<int>(3, mdata->t_.month);
  pro.set_output_val<int>(4, mdata->t_.day);
  pro.set_output_val<int>(5, mdata->t_.hour);
  pro.set_output_val<int>(6, mdata->t_.min);
  pro.set_output_val<int>(7, mdata->t_.sec);
  pro.set_output_val<float>(8, mdata->gsd_);
  pro.set_output_val<vcl_string>(9, mdata->satellite_name_);
  return true;
}

