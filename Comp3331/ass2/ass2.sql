-- COMP3311 20T3 Assignment 2

-- Q1: students who've studied many courses

create view Q1(unswid,name)
as
SELECT people.unswid,people.name from people,Course_enrolments
where people.id = Course_enrolments.student
GROUP BY Course_enrolments.student having count(*) > 65;

-- Q2: numbers of students, staff and both

create or replace view Q2(nstudents,nstaff,nboth)
as
select t1.nstudents,t2.nstaff,t3.nboth
from (select count(*) nstudents from Students) t1,
     (select count(*) nstaff from Staff) t2 ,
     (select count(*) nboth from Students,Staff where Students.id = Staff.id);


-- Q3: prolific Course Convenor(s)

create or replace view Q3(name,ncourses)
as
select name,t1.ncourses
from people,
    (select count(*) ncourses from Course_staff
    where (select id from Staff_roles where name = 'Course Convenor' limit 1) = role
    GROUP BY staff
    order by count(*) DESC
    limit 1) t1
where
    (select staff from Course_staff
    where (select id from Staff_roles where name = 'Course Convenor' limit 1) = role
    GROUP BY staff
    order by count(*) DESC
    limit 1) = people.id;

-- Q4: Comp Sci students in 05s2 and 17s1

create or replace view Q4a(id,name)
as
select unswid,name
from people,
     (select student from program_enrolments
     where
           (select code from Programs where id = program) = '3978' and
           (select year from Terms where term = id) = 2005 and
           (select session from Terms where term = id) = 'S2'
     ) t1
where t1.student = id
;

create or replace view Q4b(id,name)
as
select unswid,name
from people,
     (select student from program_enrolments
     where (select code from Programs where id = program) = '3778' and
           (select year from Terms where term = id) = 2017 and
           (select session from Terms where term = id) = 'S1'
     ) t1
where t1.student = id
;

-- Q5: most "committee"d faculty

create or replace view Q5(name)
as
select name from Orgunits,
    (select facultyOf(id)
    from OrgUnits
    where (select id from OrgUnit_types where name = 'Committee') = utype
    group by facultyOf(id)
    order by count(facultyOf(id)) DESC
    limit 1 ) t1
where t1.facultyOf = id
;

-- Q6: nameOf function

create or replace function
   Q6(id integer) returns text
as $$
begin
   select unswid,id,name
   from people
   if (people.id = id or unswid = id) then
      return name;
   else
      return '';
   end if;
end;
$$ language sql;

-- Q7: offerings of a subject

create or replace function
   Q7(subject text)
     returns table (subject text, term text, convenor text)
as $$

   RETURN (select subjects.code as subject,termname(terms.id) as term,t2.name as convenor
   from subjects,terms,
      (select people.name,Courses.subject ,Courses.term
      from people,Courses,
         (select course,staff from Course_staff
         where (select id from Staff_roles where name = 'Course Convenor' limit 1) = role
         ) t1
      where
         people.id = t1.staff and Courses.id = t1.course) t2
   where subjects.id = t2.subject and terms.id = t2.term and subjects.code = q7.subject);


    $$ language sql;
-- Q8: transcript

create or replace function
   Q8(zid integer) returns setof TranscriptRecord
as $$
...
$$ language plpgsql;

-- Q9: members of academic object group

create or replace function
   Q9(gid integer) returns setof AcObjRecord
as $$
...
$$ language plpgsql;

-- Q10: follow-on courses

create or replace function
   Q10(code text) returns setof text
as $$
declare
    setof text;
begin
    select code into setof
    from Subject where longname =
                       (
                           select name
                           from Acad_object_groups
                           where id = (
                               select ao_group
                               from Rules
                               where id = (
                                   select rule
                                   from subject_prereqs
                                   where subject = (
                                       select id
                                       from Subjects
                                       where code = ’Q10.code‘
                                   )
                               )
                           )
                       );
    return setof;
end;
$$ language plpgsql;
