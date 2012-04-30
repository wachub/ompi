/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2011-2012 Cisco Systems, Inc.  All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 */

#include "ompi_config.h"

#include "ompi/mpi/fortran/mpif-h/bindings.h"
#include "ompi/mpi/fortran/base/constants.h"
#include "ompi/communicator/communicator.h"

#if OPAL_HAVE_WEAK_SYMBOLS && OMPI_PROFILE_LAYER
#pragma weak PMPI_RECV = ompi_recv_f
#pragma weak pmpi_recv = ompi_recv_f
#pragma weak pmpi_recv_ = ompi_recv_f
#pragma weak pmpi_recv__ = ompi_recv_f

#pragma weak PMPI_Recv_f = ompi_recv_f
#pragma weak PMPI_Recv_f08 = ompi_recv_f
#elif OMPI_PROFILE_LAYER
OMPI_GENERATE_F77_BINDINGS (PMPI_RECV,
                           pmpi_recv,
                           pmpi_recv_,
                           pmpi_recv__,
                           pompi_recv_f,
                           (char *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr),
                           (buf, count, datatype, source, tag, comm, status, ierr) )
#endif

#if OPAL_HAVE_WEAK_SYMBOLS
#pragma weak MPI_RECV = ompi_recv_f
#pragma weak mpi_recv = ompi_recv_f
#pragma weak mpi_recv_ = ompi_recv_f
#pragma weak mpi_recv__ = ompi_recv_f

#pragma weak MPI_Recv_f = ompi_recv_f
#pragma weak MPI_Recv_f08 = ompi_recv_f
#endif

#if ! OPAL_HAVE_WEAK_SYMBOLS && ! OMPI_PROFILE_LAYER
OMPI_GENERATE_F77_BINDINGS (MPI_RECV,
                           mpi_recv,
                           mpi_recv_,
                           mpi_recv__,
                           ompi_recv_f,
                           (char *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr),
                           (buf, count, datatype, source, tag, comm, status, ierr) )
#endif


#if OMPI_PROFILE_LAYER && ! OPAL_HAVE_WEAK_SYMBOLS
#include "ompi/mpi/fortran/mpif-h/profile/defines.h"
#endif

void ompi_recv_f(char *buf, MPI_Fint *count, MPI_Fint *datatype, 
                MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, 
                MPI_Fint *status, MPI_Fint *ierr)
{
   MPI_Status *c_status;
#if OMPI_SIZEOF_FORTRAN_INTEGER != SIZEOF_INT
   MPI_Status c_status2;
#endif
   MPI_Comm c_comm = MPI_Comm_f2c(*comm);
   MPI_Datatype c_type = MPI_Type_f2c(*datatype);
   int c_ierr;

   /* See if we got MPI_STATUS_IGNORE */
   if (OMPI_IS_FORTRAN_STATUS_IGNORE(status)) {
      c_status = MPI_STATUS_IGNORE;
   } else {
      /* If sizeof(int) == sizeof(INTEGER), then there's no
         translation necessary -- let the underlying functions write
         directly into the Fortran status */

#if OMPI_SIZEOF_FORTRAN_INTEGER == SIZEOF_INT
      c_status = (MPI_Status *) status;
#else
      c_status = &c_status2;
#endif
   }

   /* Call the C function */
   c_ierr = MPI_Recv(OMPI_F2C_BOTTOM(buf), OMPI_FINT_2_INT(*count),
                     c_type, OMPI_FINT_2_INT(*source), 
                     OMPI_FINT_2_INT(*tag), c_comm,
                     c_status);
   if (NULL != ierr) *ierr = OMPI_INT_2_FINT(c_ierr);

#if OMPI_SIZEOF_FORTRAN_INTEGER != SIZEOF_INT
   if (MPI_SUCCESS == c_ierr && MPI_STATUS_IGNORE != c_status) {
       MPI_Status_c2f(c_status, status);
   }
#endif
}