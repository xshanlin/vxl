#include <rrel/rrel_orthogonal_regression.h>

#include <vnl/vnl_matrix.h>
#include <vnl/vnl_math.h>
#include <vnl/vnl_vector.h>
#include <vnl/algo/vnl_svd.h>

#include <vcl_iostream.h>
#include <vcl_vector.h>

rrel_orthogonal_regression::rrel_orthogonal_regression( const vnl_matrix<double>& pts )
  : vars_( pts )
{
  unsigned int num_pts = pts.rows ();
  set_param_dof( pts.cols() );
  if ( param_dof() > num_pts ) {
    vcl_cerr << "\nrrel_orthogonal_regression::rrel_orthogonal_regression  WARNING:  DoF is greater than\n"
         << "the number of data points.  An infinite set of equally valid\n"
         << "solutions exists.\n";
  }
  set_num_samples_for_fit( param_dof() );
}


rrel_orthogonal_regression::rrel_orthogonal_regression( const vcl_vector<vnl_vector<double> >& pts )
  : vars_( pts.size(),pts[0].size() )
{
  unsigned int num_pts = vars_.rows();

  for (unsigned int i=0;i<num_pts;i++)
    vars_.set_row(i, pts[i]);

  set_param_dof( vars_.cols() ); // up to a scale
  if ( param_dof() > num_pts ) {
    vcl_cerr << "\nrrel_orthogonal_regression::rrel_orthogonal_regression  WARNING:  DoF is greater than\n"
         << "the number of data points.  An infinite set of equally valid\n"
         << "solutions exists.\n";
  }
  set_num_samples_for_fit( param_dof() );
}


rrel_orthogonal_regression::~rrel_orthogonal_regression()
{
}

unsigned int
rrel_orthogonal_regression::num_samples( ) const
{
  return vars_.rows();
}


bool
rrel_orthogonal_regression::fit_from_minimal_set( const vcl_vector<int>& point_indices,
                                                  vnl_vector<double>& params ) const
{
  if ( point_indices.size() != param_dof() ) {
    vcl_cerr << "rrel_orthogonal_regression::fit_from_minimal_sample  The number of point "
         << "indices must agree with the fit degrees of freedom.\n";
    return false;
  }

  // The equation to be solved is Ap = 0, where A is a dof_ x dof_
  // because the solution is up to a scale.
  // The plane equation will be p(x - x0) = 0
  // where x0 is the center of the points.
  // We solve the uniqueness by adding another constraint ||p||=1

  vnl_matrix<double> A(param_dof(), param_dof());
  vnl_vector<double> sum_vect(3, 0.0);
  for ( unsigned int i=0; i<param_dof(); ++i ) {
    int index = point_indices[i];
    sum_vect += vars_.get_row(index);
  }
  vnl_vector<double> avg = sum_vect / param_dof();
  for (unsigned int i=0; i<param_dof(); ++i)
    A.set_row(i, vars_.get_row(i)-avg);

  vnl_svd<double> svd( A, 1.0e-8 );
  if ( (unsigned int)svd.rank() < param_dof() ) {
    vcl_cerr << "rrel_orthogonal_regression:: singular fit!\n";
    return false;    // singular fit
  }
  else {
    vnl_vector<double> norm = svd.nullvector();
    params.resize( norm.size() + 1 );
    for (unsigned int i=0; i<norm.size(); i++)
      params[i] = norm[i];
    params[ norm.size() ] = -1 * dot_product( norm, avg );
    //params /= vcl_sqrt( 1 - vnl_math_sqr( params[ params.size()-1 ] ) );  // normal is a unit vector
  }
  return true;
}


void
rrel_orthogonal_regression::compute_residuals( const vnl_vector<double>& params,
                                               vcl_vector<double>& residuals ) const
{
  // The residual is the algebraic distance, which is simply A * p.
  // Assumes the parameter vector has a unit normal.
  if ( residuals.size() != vars_.rows())
    residuals.resize( vars_.rows() );
  vnl_vector<double> norm(params.size()-1);
  for (unsigned int i=0; i<params.size()-1; i++)
    norm[i] = params[i];

  for ( unsigned int i=0; i<vars_.rows(); ++i ) {
    residuals[i] = dot_product(norm, vars_.get_row(i) ) + params[params.size()-1];
  }
}


// Compute a least-squares fit, using the weights if they are provided.
// The cofact matrix is not used or set.
bool
rrel_orthogonal_regression::weighted_least_squares_fit( vnl_vector<double>& params,
                                                        vnl_matrix<double>& cofact,
                                                        vcl_vector<double> *weights ) const
{
  // If params and cofact are NULL pointers and the fit is successful,
  // this function will allocate a new vector and a new
  // matrix. Otherwise, it assumes that *params is a param_dof() x 1 vector
  // and that cofact is param_dof() x param_dof() matrix.

  assert( !weights || weights->size() == vars_.rows() );

  vnl_vector<double> sum_vect(3, 0.0);
  vnl_matrix<double> A(vars_.cols(), vars_.cols());
  vnl_vector<double> avg;
  vnl_matrix<double> shift_vars( vars_.rows(), vars_.cols() );

  if (weights) {
    double sum_weight=0;
    for (unsigned int i=0; i<vars_.rows(); ++i) {
      sum_vect += vars_.get_row(i) * (*weights)[i];
      sum_weight += (*weights)[i];
    }
    avg = sum_vect / sum_weight;
    for (unsigned int i=0; i<vars_.rows(); ++i)
      shift_vars.set_row(i, (vars_.get_row(i)-avg) * vcl_sqrt((*weights)[i]));
  }
  else {
    for (unsigned int i=0; i<vars_.rows(); ++i)
      sum_vect += vars_.get_row(i);
    avg = sum_vect / vars_.rows();
    for (unsigned int i=0; i<vars_.rows(); ++i)
      shift_vars.set_row(i, vars_.get_row(i) -avg);
  }

  A = shift_vars.transpose() * shift_vars;

  vnl_svd<double> svd( A, 1.0e-8 );
  if ( (unsigned int)svd.rank() < param_dof() ) {
    vcl_cerr << "rrel_orthogonal_regression::WeightedLeastSquaresFit --- singularity!\n";
    return false;
  }
  else {
    vnl_vector<double> norm = svd.nullvector();
    params.resize(norm.size()+1);
    for (unsigned int i=0; i<norm.size(); ++i) {
      params[i] = norm[i];
    }
    params[norm.size()] = -1*dot_product(norm, avg);
    return true;
  }
}


void
rrel_orthogonal_regression::print_points() const
{
  vcl_cout << "\nrrel_orthogonal_regression::print_points:\n"
           << "  param_dof() = " << param_dof() << "\n"
           << "  num_pts = " << vars_.rows() << "\n\n"
           << " i   vars_ \n"
           << " =   ========= \n";
  for ( unsigned int i=0; i<vars_.rows(); ++i ) {
    vcl_cout << " " << i << "   " << vars_.get_row (i) << "\n";
  }
}
